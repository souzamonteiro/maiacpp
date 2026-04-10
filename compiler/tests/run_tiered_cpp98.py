#!/usr/bin/env python3
import argparse
import json
import os
import re
import subprocess
import sys
import tempfile
from datetime import datetime, timezone
from pathlib import Path


def parse_conformance_matrix(matrix_path: Path):
    entries = []
    line_rx = re.compile(r"^-\s+(.+):\s+(done|partial|missing)\s*$")
    for raw in matrix_path.read_text(encoding="utf-8").splitlines():
        m = line_rx.match(raw.strip())
        if not m:
            continue
        entries.append({"name": m.group(1), "status": m.group(2)})

    done = sum(1 for e in entries if e["status"] == "done")
    partial = sum(1 for e in entries if e["status"] == "partial")
    missing = sum(1 for e in entries if e["status"] == "missing")
    total = len(entries)
    weighted = done + 0.5 * partial
    weighted_pct = (100.0 * weighted / total) if total else 0.0

    return {
        "total": total,
        "done": done,
        "partial": partial,
        "missing": missing,
        "weighted": weighted,
        "weightedPct": round(weighted_pct, 1),
        "entries": entries,
    }


def run_case(case, repo_root: Path, tmp_dir: Path):
    cmd = [arg.replace("{tmpDir}", str(tmp_dir)) for arg in case["command"]]
    proc = subprocess.run(
        cmd,
        cwd=str(repo_root),
        capture_output=True,
        text=True,
    )

    errors = []

    expected_code = int(case.get("expectExitCode", 0))
    if proc.returncode != expected_code:
        errors.append(f"expected exit code {expected_code}, got {proc.returncode}")

    for snippet in case.get("stdoutContains", []):
        if snippet not in proc.stdout:
            errors.append(f"missing stdout snippet: {snippet}")

    for snippet in case.get("stderrContains", []):
        if snippet not in proc.stderr:
            errors.append(f"missing stderr snippet: {snippet}")

    return {
        "id": case["id"],
        "family": case.get("family", "unclassified"),
        "matrixFamily": case.get("matrixFamily"),
        "command": cmd,
        "exitCode": proc.returncode,
        "ok": len(errors) == 0,
        "errors": errors,
        "stdoutPreview": "\n".join(proc.stdout.splitlines()[-8:]),
        "stderrPreview": "\n".join(proc.stderr.splitlines()[-8:]),
    }


def run_tiers(plan, repo_root: Path):
    results = {"tier1": [], "tier2": [], "tier3": []}
    with tempfile.TemporaryDirectory(prefix="maiacpp_tiers_") as temp_dir:
        tmp_dir = Path(temp_dir)
        for tier_name in ["tier1", "tier2", "tier3"]:
            for case in plan["tiers"].get(tier_name, []):
                results[tier_name].append(run_case(case, repo_root, tmp_dir))
    return results


def summarize_tier(items):
    total = len(items)
    ok = sum(1 for i in items if i["ok"])
    failed = total - ok
    return {
        "total": total,
        "ok": ok,
        "failed": failed,
        "passRatePct": round((100.0 * ok / total), 1) if total else 0.0,
    }


def summarize_matrix_tracking(matrix, tier_results):
    matrix_names = [e["name"] for e in matrix.get("entries", [])]
    matrix_name_set = set(matrix_names)

    tracked = {}
    for tier_name in ["tier1", "tier2", "tier3"]:
        for item in tier_results.get(tier_name, []):
            fam = item.get("matrixFamily")
            if not fam or fam not in matrix_name_set:
                continue
            bucket = tracked.get(fam)
            if not bucket:
                bucket = {"family": fam, "cases": 0, "ok": 0, "failed": 0}
                tracked[fam] = bucket
            bucket["cases"] += 1
            if item.get("ok"):
                bucket["ok"] += 1
            else:
                bucket["failed"] += 1

    tracked_names = sorted(tracked.keys())
    untracked_names = [name for name in matrix_names if name not in tracked]

    return {
        "trackedFamilies": len(tracked_names),
        "totalFamilies": len(matrix_names),
        "trackedPct": round((100.0 * len(tracked_names) / len(matrix_names)), 1) if matrix_names else 0.0,
        "tracked": [tracked[name] for name in tracked_names],
        "untracked": untracked_names,
    }


def main():
    parser = argparse.ArgumentParser(description="Run MaiaCpp tiered C++98 suite and emit machine-readable report")
    parser.add_argument("--plan", default="compiler/tests/ebnf_tiers.json")
    parser.add_argument("--matrix", default="docs/CONFORMANCE_MATRIX.md")
    parser.add_argument("--out", default="out/reports/ebnf-tiered-report.json")
    args = parser.parse_args()

    repo_root = Path.cwd()
    plan_path = (repo_root / args.plan).resolve()
    matrix_path = (repo_root / args.matrix).resolve()
    out_path = (repo_root / args.out).resolve()

    if not plan_path.exists():
        print(f"[fail] plan not found: {plan_path}")
        return 2
    if not matrix_path.exists():
        print(f"[fail] matrix not found: {matrix_path}")
        return 2

    plan = json.loads(plan_path.read_text(encoding="utf-8"))
    matrix = parse_conformance_matrix(matrix_path)
    tier_results = run_tiers(plan, repo_root)

    summary = {
        "tier1": summarize_tier(tier_results["tier1"]),
        "tier2": summarize_tier(tier_results["tier2"]),
        "tier3": summarize_tier(tier_results["tier3"]),
    }
    matrix_tracking = summarize_matrix_tracking(matrix, tier_results)

    report = {
        "generatedAt": datetime.now(timezone.utc).isoformat(),
        "planPath": str(plan_path.relative_to(repo_root)),
        "matrixPath": str(matrix_path.relative_to(repo_root)),
        "matrixCoverage": {
            "totalFamilies": matrix["total"],
            "done": matrix["done"],
            "partial": matrix["partial"],
            "missing": matrix["missing"],
            "weightedImplementedPct": matrix["weightedPct"],
        },
        "tierSummary": summary,
        "matrixTracking": matrix_tracking,
        "tierResults": tier_results,
    }

    out_path.parent.mkdir(parents=True, exist_ok=True)
    out_path.write_text(json.dumps(report, indent=2), encoding="utf-8")

    print("Tiered C++98 Report")
    print(f"- Matrix weighted coverage: {matrix['weightedPct']}% ({matrix['done']} done, {matrix['partial']} partial, {matrix['missing']} missing)")
    print(f"- Tier 1: {summary['tier1']['ok']}/{summary['tier1']['total']} passed")
    print(f"- Tier 2: {summary['tier2']['ok']}/{summary['tier2']['total']} passed")
    print(f"- Tier 3: {summary['tier3']['ok']}/{summary['tier3']['total']} passed")
    print(f"- Matrix families tracked by tier cases: {matrix_tracking['trackedFamilies']}/{matrix_tracking['totalFamilies']} ({matrix_tracking['trackedPct']}%)")
    print(f"- Report JSON: {out_path.relative_to(repo_root)}")

    # Non-zero when Tier 1 or Tier 2 fail. Tier 3 can fail while being tracked.
    if summary["tier1"]["failed"] > 0 or summary["tier2"]["failed"] > 0:
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main())
