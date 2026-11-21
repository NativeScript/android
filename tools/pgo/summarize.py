#!/usr/bin/env python3
"""Summarizes runs from ab.sh and startup.sh.

usage:
  summarize.py ab <outdir> <labelA> <labelB> [row-substring ...]
      Per benchmark row: the median across launches of each launch's median
      (and of each launch's min) for A and B, the range across launches, and
      the B-vs-A delta. Only measured relaunches (<label>.<round>.txt) count;
      the first-<label>.* launches after each install are ignored.
  summarize.py startup <outfile>
      Median, min and max cold-start TotalTime per label.
"""
import glob
import json
import os
import statistics
import sys


def load_runs(outdir, label):
    runs = []
    for path in sorted(glob.glob(os.path.join(outdir, label + ".*.txt"))):
        rows = {}
        with open(path) as f:
            for line in f:
                i = line.find("JNIBENCH {")
                if i >= 0:
                    row = json.loads(line[i + len("JNIBENCH "):])
                    rows[row["name"]] = row
        runs.append(rows)
    return runs


def across(runs, name, key):
    values = [r[name][key] for r in runs if name in r and key in r[name]]
    if not values:
        return None
    return statistics.median(values), min(values), max(values)


def ab(outdir, a, b, filters):
    ra, rb = load_runs(outdir, a), load_runs(outdir, b)
    if not ra or not rb:
        sys.exit(f"no runs for {a if not ra else b} in {outdir}")
    print(f"{len(ra)} launches of {a}, {len(rb)} of {b}; ns per call")
    print(f"{'row':52s} | {a:>8s} (range) | {b:>8s} (range) | delta median   | delta min")
    for name in ra[0]:
        if filters and not any(f in name for f in filters):
            continue
        sa, sb = across(ra, name, "median"), across(rb, name, "median")
        ma, mb = across(ra, name, "min"), across(rb, name, "min")
        if not sa or not sb or not ma or not mb:
            continue
        d = sb[0] - sa[0]
        dmin = mb[0] - ma[0]
        print(
            f"{name[:52]:52s} | {sa[0]:8.1f} ({sa[1]:.0f}..{sa[2]:.0f}) | {sb[0]:8.1f} ({sb[1]:.0f}..{sb[2]:.0f})"
            f" | {d:+7.1f} {100 * d / sa[0]:+5.1f}% | {dmin:+7.1f} {100 * dmin / ma[0]:+5.1f}%"
        )


def startup(path):
    times = {}
    with open(path) as f:
        for line in f:
            parts = line.split()
            if len(parts) == 4 and all(p.isdigit() for p in parts[1:]):
                times.setdefault(parts[0], []).append(int(parts[3]))
    for label, values in times.items():
        print(f"{label}: median {statistics.median(values)} ms, min {min(values)}, max {max(values)}, n={len(values)}")


def main(argv):
    if len(argv) >= 5 and argv[1] == "ab":
        ab(argv[2], argv[3], argv[4], argv[5:])
    elif len(argv) == 3 and argv[1] == "startup":
        startup(argv[2])
    else:
        sys.exit(__doc__)


if __name__ == "__main__":
    main(sys.argv)
