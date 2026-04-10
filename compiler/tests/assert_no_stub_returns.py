#!/usr/bin/env python3
import argparse
import re
import subprocess
import sys
import tempfile
from pathlib import Path


def extract_function_bodies(c_text):
    lines = c_text.splitlines()
    out = {}
    i = 0
    while i < len(lines):
        line = lines[i]
        m = re.match(r"^\s*[A-Za-z_][\w\s\*]*\s+([A-Za-z_][A-Za-z0-9_]*)\s*\([^;]*\)\s*\{\s*$", line)
        if not m:
            i += 1
            continue

        name = m.group(1)
        brace_depth = 1
        body_lines = []
        i += 1
        while i < len(lines) and brace_depth > 0:
            cur = lines[i]
            brace_depth += cur.count("{")
            brace_depth -= cur.count("}")
            if brace_depth > 0:
                body_lines.append(cur)
            i += 1

        out[name] = "\n".join(body_lines)

    return out


def main():
    parser = argparse.ArgumentParser(description="Assert selected emitted C functions do not contain stub returns")
    parser.add_argument("--file", required=True, help="Input C++ source file")
    parser.add_argument("--functions", required=True, help="Comma-separated emitted C function names")
    args = parser.parse_args()

    repo_root = Path.cwd()
    input_cpp = (repo_root / args.file).resolve()
    compiler_js = repo_root / "compiler" / "cpp-compiler.js"

    if not input_cpp.exists():
        print(f"[fail] input not found: {input_cpp}")
        return 2
    if not compiler_js.exists():
        print(f"[fail] compiler not found: {compiler_js}")
        return 2

    target_functions = [f.strip() for f in args.functions.split(",") if f.strip()]
    if not target_functions:
        print("[fail] no target functions provided")
        return 2

    with tempfile.TemporaryDirectory(prefix="maiacpp_stub_guard_") as temp_dir:
        c_out = Path(temp_dir) / "out.c"
        proc = subprocess.run(
            ["node", str(compiler_js), str(input_cpp), "--output", str(c_out)],
            cwd=str(repo_root),
            capture_output=True,
            text=True,
        )
        if proc.returncode != 0:
            print("[fail] could not generate C output")
            print(proc.stdout)
            print(proc.stderr)
            return 1

        c_text = c_out.read_text(encoding="utf-8")
        bodies = extract_function_bodies(c_text)

        failures = []
        for fn in target_functions:
            body = bodies.get(fn)
            if body is None:
                failures.append(f"function not found in emitted C: {fn}")
                continue
            if "return (int)0;" in body:
                failures.append(f"stub return found in function: {fn}")

        if failures:
            print("[fail] stub guard violations")
            for f in failures:
                print(f"- {f}")
            return 1

    print("[ok] selected emitted C functions have no stub return")
    return 0


if __name__ == "__main__":
    sys.exit(main())
