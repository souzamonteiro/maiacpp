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


def run_fixture(parser: Path, input_file: Path, spec: dict):
    should_parse = bool(spec.get("shouldParse", True))
    cmd = ["node", str(parser), str(input_file)]
    proc = subprocess.run(cmd, capture_output=True, text=True)

    errors = []

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


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--fixtures-dir", default="tests/fixtures")
    parser.add_argument("--parser", default="Cpp-main.js")
    args = parser.parse_args()

    root = Path.cwd()
    fixtures_dir = (root / args.fixtures_dir).resolve()
    parser_path = (root / args.parser).resolve()

    if not parser_path.exists():
        print(f"[fail] parser not found: {parser_path}")
        return 2

    specs = load_fixture_specs(fixtures_dir)
    if not specs:
        print(f"[fail] no fixtures found in {fixtures_dir}")
        return 2

    failures = 0

    for case_stem, input_file, spec in specs:
        errors = run_fixture(parser_path, input_file, spec)
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
