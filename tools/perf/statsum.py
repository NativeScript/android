#!/usr/bin/env python3
"""Per-operation counter table from stat.sh runs.

usage: tools/perf/statsum.py DIR --anchor LABEL [--labels A,B,...] [--md]

For every row key and label: (count at scale2 - count at scale1) on the JS thread,
divided by (ops at scale2 - ops at scale1), where ops is the "ops" field the bench
reports for the row. Rows without "ops" get the raw count difference instead. Values are
medians over passes; the delta columns are label - anchor, so an A/A label's delta is
the noise floor.

A count marked "mux" was multiplexed (the CPU could not count every event at once) and
is an estimate; drop an event and rerun if it matters.
"""
import argparse
import glob
import json
import os
import re
import statistics
import sys

COUNT = re.compile(r'^\s*(.+?)\s+(\d+)\s+(\d+)\s+([\d,]+)\s+(\S+)\s+#.*\((\d+(?:\.\d+)?)%\)\s*$')
FILE = re.compile(r'^(.+)\.([^.]+)\.(\d+)\.p(\d+)\.txt$')


def read_counts(path):
    out, mux = {}, False
    with open(path, errors='replace') as f:
        for line in f:
            m = COUNT.match(line)
            # The app's main thread (pid == tid) is the JS thread.
            if m and m.group(2) == m.group(3):
                out[m.group(5)] = out.get(m.group(5), 0) + int(m.group(4).replace(',', ''))
                if float(m.group(6)) < 100:
                    mux = True
    return out, mux


def read_ops(path):
    ops, names = 0, []
    if not os.path.exists(path):
        return None, names
    with open(path, errors='replace') as f:
        for line in f:
            m = re.search(r'\b[A-Z][A-Z0-9]* (\{.*\})\s*$', line)
            if not m:
                continue
            try:
                j = json.loads(m.group(1))
            except ValueError:
                continue
            if 'name' in j:
                names.append(j['name'])
                ops += j.get('ops', 0)
    return (ops or None), names


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument('dir')
    ap.add_argument('--anchor', required=True)
    ap.add_argument('--labels')
    ap.add_argument('--md', action='store_true')
    a = ap.parse_args()

    runs = {}
    for path in glob.glob(os.path.join(a.dir, '*.txt')):
        m = FILE.match(os.path.basename(path))
        if m:
            runs.setdefault((m.group(1), m.group(2), int(m.group(4))), {})[int(m.group(3))] = path
    if not runs:
        sys.exit('no stat runs in ' + a.dir)
    found = sorted({k[0] for k in runs})
    labels = a.labels.split(',') if a.labels else [a.anchor] + [l for l in found if l != a.anchor]
    keys = sorted({k[1] for k in runs})
    events = []
    values = {}
    notes = set()
    for (label, key, p), by_scale in runs.items():
        if len(by_scale) != 2:
            notes.add('%s %s pass %d: needs exactly two scales, has %s' % (label, key, p, sorted(by_scale)))
            continue
        s1, s2 = sorted(by_scale)
        c1, mux1 = read_counts(by_scale[s1])
        c2, mux2 = read_counts(by_scale[s2])
        o1, n1 = read_ops(by_scale[s1][:-4] + '.bench')
        o2, n2 = read_ops(by_scale[s2][:-4] + '.bench')
        if len(set(n2)) > 1:
            notes.add('%s: filter matched %d rows (%s); per-op values mix them' % (key, len(set(n2)), ', '.join(sorted(set(n2)))))
        per_op = o1 is not None and o2 is not None and o2 > o1
        if not per_op:
            notes.add('%s: no ops reported; values are raw count differences' % key)
        for ev in c1:
            if ev not in c2:
                continue
            if ev not in events:
                events.append(ev)
            d = c2[ev] - c1[ev]
            v = d / (o2 - o1) if per_op else float(d)
            values.setdefault((label, key, ev), []).append((v, mux1 or mux2))

    head = ['row / event'] + labels + ['%s-%s' % (l, a.anchor) for l in labels if l != a.anchor]
    rows = []
    for key in keys:
        for ev in events:
            cells = ['%s %s' % (key, ev)]
            med = {}
            for l in labels:
                vs = values.get((l, key, ev))
                if not vs:
                    cells.append('-')
                    continue
                med[l] = statistics.median(v for v, _ in vs)
                spread = (' [%.1f-%.1f]' % (min(v for v, _ in vs), max(v for v, _ in vs))) if len(vs) > 1 else ''
                cells.append('%.1f%s%s' % (med[l], spread, ' mux' if any(x for _, x in vs) else ''))
            for l in labels:
                if l == a.anchor:
                    continue
                cells.append(('%+.1f' % (med[l] - med[a.anchor])) if l in med and a.anchor in med else '-')
            rows.append(cells)
    if a.md:
        print('| ' + ' | '.join(head) + ' |')
        print('|---|' + '---:|' * (len(head) - 1))
        for r in rows:
            print('| ' + ' | '.join(r) + ' |')
    else:
        widths = [max(len(r[i]) for r in [head] + rows) for i in range(len(head))]
        for r in [head] + rows:
            print('  '.join(('%-*s' if i == 0 else '%*s') % (widths[i], c) for i, c in enumerate(r)))
    for n in sorted(notes):
        print('note: ' + n)


if __name__ == '__main__':
    main()
