#!/usr/bin/env python3
import argparse
import difflib
import re
import shutil
import subprocess
import sys
import tempfile
from pathlib import Path


def choose_cxx():
    for candidate in ["clang++", "g++"]:
        if shutil.which(candidate):
            return candidate
    return None


def run_cmd(cmd, cwd):
    return subprocess.run(cmd, cwd=str(cwd), capture_output=True, text=True)


def parse_node_main_rc(stdout_text):
    m = re.search(r"\[node\]\s+main\(\)\s+=>\s+(-?\d+)\s*$", stdout_text, re.MULTILINE)
    if not m:
        return None
    return int(m.group(1))


def parse_browser_main_rc(stdout_text):
    m = re.search(r"\[browser-host\]\s+main\(\)\s+=>\s+(-?\d+)\s*$", stdout_text, re.MULTILINE)
    if not m:
        return None
    return int(m.group(1))


def normalize_node_program_output(stdout_text):
    lines = stdout_text.splitlines()
    out = []
    for ln in lines:
        if ln.startswith("Parsing: "):
            continue
        if ln.startswith("Parser: "):
            continue
        if ln.startswith("C gerado em: "):
            continue
        if ln.startswith("[node] wasm: "):
            continue
        if ln.startswith("[node] main() => "):
            continue
        out.append(ln)
    return "\n".join(out).strip()


def normalize_browser_program_output(stdout_text):
    lines = stdout_text.splitlines()
    out = []
    for ln in lines:
        if ln.startswith("Parsing: "):
            continue
        if ln.startswith("Parser: "):
            continue
        if ln.startswith("C gerado em: "):
            continue
        if ln.startswith("[browser-host] main() => "):
            continue
        out.append(ln)
    return "\n".join(out).strip()


def normalize_native_output(stdout_text):
    return "\n".join(stdout_text.splitlines()).strip()


def main():
    parser = argparse.ArgumentParser(description="Compare native C++ output with MaiaCpp->MaiaC->WASM output")
    parser.add_argument("--file", default="./compiler/test_cpp98_extended.cpp")
    parser.add_argument("--out-dir", default="./out/reports/cpp-vs-c")
    parser.add_argument("--runtime", choices=["node", "browser-host"], default="node")
    parser.add_argument("--keep-temp", action="store_true")
    args = parser.parse_args()

    repo_root = Path.cwd()
    input_cpp = (repo_root / args.file).resolve()
    out_dir = (repo_root / args.out_dir).resolve()

    if not input_cpp.exists():
        print(f"[fail] input not found: {input_cpp}")
        return 2

    cxx = choose_cxx()
    if not cxx:
        print("[fail] no C++ compiler found (clang++/g++)")
        return 2

    cpp_compiler = repo_root / "compiler" / "cpp-compiler.js"
    webcpp = repo_root / "bin" / "webcpp.sh"
    node_runner = repo_root / "tools" / "node" / "run-wasm-node.js"
    browser_runner = repo_root / "tools" / "browser" / "run-wasm-browser-host.js"

    required_tools = [cpp_compiler, webcpp]
    if args.runtime == "node":
        required_tools.append(node_runner)
    else:
        required_tools.append(browser_runner)

    for required in required_tools:
        if not required.exists():
            print(f"[fail] required tool missing: {required}")
            return 2

    out_dir.mkdir(parents=True, exist_ok=True)
    stem = input_cpp.stem
    generated_c = out_dir / f"{stem}.generated.c"

    gen_proc = run_cmd(["node", str(cpp_compiler), str(input_cpp), "--output", str(generated_c)], repo_root)
    if gen_proc.returncode != 0:
        print("[fail] failed to generate C from C++")
        print(gen_proc.stdout)
        print(gen_proc.stderr)
        return 1

    with tempfile.TemporaryDirectory(prefix="maiacpp_compare_") as temp_dir:
        temp = Path(temp_dir)
        native_bin = temp / f"{stem}.native"
        wasm_file = temp / f"{stem}.wasm"

        native_build = run_cmd([cxx, "-std=c++98", "-O0", str(input_cpp), "-o", str(native_bin)], repo_root)
        if native_build.returncode != 0:
            print("[fail] native C++ compilation failed")
            print(native_build.stdout)
            print(native_build.stderr)
            return 1

        native_run = run_cmd([str(native_bin)], repo_root)

        wasm_build = run_cmd(["bash", str(webcpp), "--file", str(input_cpp), "--wasm-out", str(wasm_file)], repo_root)
        if wasm_build.returncode != 0:
            print("[fail] MaiaCpp->MaiaC pipeline failed")
            print(wasm_build.stdout)
            print(wasm_build.stderr)
            return 1

        runtime_runner = node_runner if args.runtime == "node" else browser_runner
        wasm_run = run_cmd(["node", str(runtime_runner), str(wasm_file)], repo_root)
        if wasm_run.returncode != 0:
            print(f"[fail] {args.runtime} WASM runner failed")
            print(wasm_run.stdout)
            print(wasm_run.stderr)
            return 1

        native_out = normalize_native_output(native_run.stdout)
        runner_out = wasm_build.stdout + ("\n" + wasm_run.stdout if wasm_run.stdout else "")
        if args.runtime == "node":
            wasm_out = normalize_node_program_output(runner_out)
            wasm_rc = parse_node_main_rc(wasm_run.stdout)
        else:
            wasm_out = normalize_browser_program_output(runner_out)
            wasm_rc = parse_browser_main_rc(wasm_run.stdout)

        native_rc = native_run.returncode

        ok = True

        if wasm_rc is None:
            print(f"[fail] could not parse [{args.runtime}] main() return code")
            ok = False
        elif wasm_rc != native_rc:
            print(f"[fail] return code mismatch: native={native_rc}, wasm={wasm_rc}")
            ok = False

        if native_out != wasm_out:
            print("[fail] stdout mismatch between native C++ and pipeline output")
            diff = difflib.unified_diff(
                native_out.splitlines(),
                wasm_out.splitlines(),
                fromfile="native-cpp",
                tofile="maiacpp-pipeline",
                lineterm="",
            )
            for ln in diff:
                print(ln)
            ok = False

        print("C++ vs Pipeline Comparison")
        print(f"- Input: {input_cpp.relative_to(repo_root)}")
        print(f"- Generated C: {generated_c.relative_to(repo_root)}")
        print(f"- Native compiler: {cxx}")
        print(f"- Runtime host: {args.runtime}")
        if wasm_rc is not None:
            print(f"- Return code: native={native_rc}, wasm={wasm_rc}")
        print(f"- Stdout equal: {'yes' if native_out == wasm_out else 'no'}")

        if not args.keep_temp:
            pass

        if not ok:
            return 1

    print("[ok] native C++ output matches MaiaCpp pipeline output")
    return 0


if __name__ == "__main__":
    sys.exit(main())
