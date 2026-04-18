#!/usr/bin/env python3
import argparse
import json
import subprocess
import sys
from pathlib import Path
import xml.etree.ElementTree as ET


def load_fixture_specs(fixtures_dir: Path):
    specs = []
    for expect_file in sorted(fixtures_dir.glob("*.expect.json")):
        case_stem = expect_file.name[:-len(".expect.json")]
        input_file = fixtures_dir / f"{case_stem}.cpp"
        if not input_file.exists():
            raise FileNotFoundError(f"Missing input for fixture '{case_stem}': {input_file}")
        with expect_file.open("r", encoding="utf-8") as f:
            spec = json.load(f)
        specs.append((case_stem, input_file, spec))
    return specs


def count_tag(xml_text: str, tag: str) -> int:
    return xml_text.count(f"<{tag}")


def run_fixture(parser: Path, input_file: Path, spec: dict, mode: str, parser_extra_args=None):
    should_parse = bool(spec.get("shouldParse", True))
    cmd = ["node", str(parser), str(input_file)]
    if parser_extra_args:
        cmd.extend([str(a) for a in parser_extra_args])
    proc = subprocess.run(cmd, capture_output=True, text=True)

    errors = []

    if mode == "xml-parser":
        if should_parse:
            if proc.returncode != 0:
                errors.append(f"expected parse success, got exit code {proc.returncode}")
                if proc.stderr.strip():
                    errors.append(f"stderr: {proc.stderr.strip().splitlines()[0]}")
                return errors

            xml_text = proc.stdout
            if not xml_text.strip():
                errors.append("parser returned empty XML")
                return errors

            try:
                root = ET.fromstring(xml_text)
            except ET.ParseError as e:
                errors.append(f"invalid XML output: {e}")
                return errors

            if root.tag != "translationUnit":
                errors.append(f"unexpected root tag: {root.tag}")

            if root.find("EOF") is None:
                errors.append("missing EOF marker")

            for snippet in spec.get("mustContain", []):
                if snippet not in xml_text:
                    errors.append(f"missing required snippet in XML: {snippet}")

            min_counts = spec.get("minTagCount", {})
            for tag, min_count in min_counts.items():
                actual = count_tag(xml_text, str(tag))
                if actual < int(min_count):
                    errors.append(f"expected at least {min_count} occurrences of <{tag}>, got {actual}")
        else:
            if proc.returncode == 0:
                errors.append("expected parse failure, got exit code 0")
            for snippet in spec.get("stderrContains", []):
                if snippet not in proc.stderr:
                    errors.append(f"expected stderr to contain: {snippet}")
        return errors

    # Compiler mode: rely on explicit parser status markers in stdout.
    out = proc.stdout
    if should_parse:
        if proc.returncode != 0:
            errors.append(f"expected compile success, got exit code {proc.returncode}")
        if "Parser: ok" not in out:
            errors.append("expected parser success marker: 'Parser: ok'")
        if "Parser falhou" in out:
            errors.append("unexpected parser failure marker in output")
    else:
        if "Parser falhou" not in out:
            errors.append("expected parser failure marker: 'Parser falhou'")

    for snippet in spec.get("mustContainCompiler", []):
        if snippet not in out:
            errors.append(f"missing required snippet in output: {snippet}")

    for snippet in spec.get("mustNotContainCompiler", []):
        if snippet in out:
            errors.append(f"unexpected snippet in output: {snippet}")

    for snippet in spec.get("stderrContains", []):
        if snippet not in proc.stderr:
            errors.append(f"expected stderr to contain: {snippet}")

    return errors


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--fixtures-dir", default="tests/fixtures")
    parser.add_argument("--parser", default="cpp-compiler.js")
    parser.add_argument(
        "--mode",
        choices=["compiler", "xml-parser"],
        default="compiler",
        help="Validation mode: compiler output markers or legacy XML parser output",
    )
    parser.add_argument(
        "--parser-extra-arg",
        action="append",
        default=[],
        help="Extra argument passed through to cpp-compiler.js (repeatable).",
    )
    parser.add_argument(
        "--cases",
        default="",
        help="Comma-separated fixture stems to run (default: all).",
    )
    args = parser.parse_args()

    root = Path.cwd()
    fixtures_dir = (root / args.fixtures_dir).resolve()
    parser_path = (root / args.parser).resolve()

    if not parser_path.exists():
        print(f"[fail] parser not found: {parser_path}")
        return 2

    specs = load_fixture_specs(fixtures_dir)
    requested_cases = [s.strip() for s in str(args.cases or "").split(",") if s.strip()]
    if requested_cases:
        req = set(requested_cases)
        specs = [item for item in specs if item[0] in req]
        missing = [case for case in requested_cases if case not in {item[0] for item in specs}]
        if missing:
            print(f"[fail] requested fixture(s) not found: {', '.join(missing)}")
            return 2
    if not specs:
        print(f"[fail] no fixtures found in {fixtures_dir}")
        return 2

    failures = 0

    for case_stem, input_file, spec in specs:
        errors = run_fixture(parser_path, input_file, spec, args.mode, parser_extra_args=args.parser_extra_arg)
        if errors:
            failures += 1
            print(f"[fail] {case_stem}")
            for err in errors:
                print(f"  - {err}")
        else:
            print(f"[ok] {case_stem}")

    print()
    if failures:
        print(f"Fixture failures: {failures}/{len(specs)}")
        return 1

    print(f"All fixtures passed: {len(specs)}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
