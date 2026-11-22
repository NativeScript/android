#!/usr/bin/env python3
"""A/B table from run.sh launch logs.

usage: tools/perf/table.py DIR[,DIR...] --anchor LABEL [--aa LABEL] [--labels A,B,...]
                           [--metric median|min|norm] [--rows SUBSTR] [--md]

  --anchor   the base label; ratios are label / anchor within the same round
  --aa       the A/A copy of the anchor: its ratio range is the noise floor that every
             other label's range is compared with
  --labels   labels and their column order (default: every label found, anchor first)
  --metric   median (default) or min of the launch's timed rounds, or norm = median / ref
             for rows that time a reference alongside (callback rows)
  --rows     only rows whose name contains this substring ('|' separates several)
  --md       Markdown output, for PR bodies

Reads DIR/<label>_r<round>.g<group>.log, the "<TAG> {json}" lines a bench logs. Launches
run.sh rejected (*.log.bad) are skipped.

Columns:
  abs     median over launches of the per-launch value, and the number of launches
  ratio   per round label/anchor: median [min-max] over rounds
Flags after a ratio:
  =   the label's ratio range overlaps the A/A range: indistinguishable from noise
  *   the ranges do not overlap
  (both need --aa and at least 3 rounds; with fewer, no flag is printed)
  B   the label's per-launch values look bimodal (a gap of more than 20% of the median
      between two clusters); compare medians with care
"""
import argparse
import glob
import json
import os
import re
import statistics
import sys

# Fewer A/A rounds than this give no usable noise range, so no =/* flags are printed.
MIN_AA_ROUNDS = 3
LINE = re.compile(r'\b([A-Z][A-Z0-9]*) (\{.*\})\s*$')
NAME = re.compile(r'^(.+)_r(\d+)(?:\.g(\d+))?\.log$')


def load(dirs):
    data = {}
    order = []
    for d in dirs:
        for path in sorted(glob.glob(os.path.join(d, '*.log'))):
            m = NAME.match(os.path.basename(path))
            if not m:
                continue
            label, rnd = m.group(1), (d, int(m.group(2)))
            with open(path, errors='replace') as f:
                for line in f:
                    mm = LINE.search(line)
                    if not mm or mm.group(1).endswith('_DONE'):
                        continue
                    try:
                        j = json.loads(mm.group(2))
                    except ValueError:
                        continue
                    if 'name' not in j or 'median' not in j:
                        continue
                    if j['name'] not in order:
                        order.append(j['name'])
                    data.setdefault((label, rnd, j['name']), j)
    return data, order


def value(j, metric):
    if metric == 'min':
        return j.get('min')
    if metric == 'norm':
        return j['median'] / j['ref'] if j.get('ref') else None
    return j.get('median')


def bimodal(values):
    if len(values) < 4:
        return False
    v = sorted(values)
    med = statistics.median(v)
    if med <= 0:
        return False
    gaps = [(v[i + 1] - v[i], i) for i in range(len(v) - 1)]
    gap, i = max(gaps)
    low, high = i + 1, len(v) - i - 1
    return gap > 0.2 * med and min(low, high) >= max(2, len(v) // 5)


def main():
    ap = argparse.ArgumentParser(add_help=False)
    ap.add_argument('dirs')
    ap.add_argument('--anchor', required=True)
    ap.add_argument('--aa')
    ap.add_argument('--labels')
    ap.add_argument('--metric', default='median', choices=['median', 'min', 'norm'])
    ap.add_argument('--rows')
    ap.add_argument('--md', action='store_true')
    ap.add_argument('-h', '--help', action='store_true')
    a = ap.parse_args()
    if a.help:
        print(__doc__)
        return
    dirs = a.dirs.split(',')
    data, order = load(dirs)
    if not data:
        sys.exit('no bench lines under ' + a.dirs)
    found = sorted({k[0] for k in data})
    labels = a.labels.split(',') if a.labels else [a.anchor] + [l for l in found if l != a.anchor]
    for l in labels:
        if l not in found:
            sys.exit('label %s has no launches (found: %s)' % (l, ', '.join(found)))
    if a.aa and a.aa not in labels:
        sys.exit('--aa label %s is not among the labels' % a.aa)
    rounds = sorted({k[1] for k in data})
    if a.rows:
        subs = a.rows.split('|')
        order = [n for n in order if any(s in n for s in subs)]
    others = [l for l in labels if l != a.anchor]

    def per_launch(label, name):
        out = []
        for r in rounds:
            j = data.get((label, r, name))
            if j is not None:
                v = value(j, a.metric)
                if v is not None:
                    out.append(v)
        return out

    def ratios(label, name):
        out = []
        for r in rounds:
            x, y = data.get((label, r, name)), data.get((a.anchor, r, name))
            if x is None or y is None:
                continue
            vx, vy = value(x, a.metric), value(y, a.metric)
            if vx is not None and vy:
                out.append(vx / vy)
        return out

    head_abs = ['row (%s; n launches)' % a.metric] + labels
    head_ratio = ['ratio to %s within a round: median [min-max]' % a.anchor] + others
    rows_abs, rows_ratio = [], []
    for name in order:
        cells = [name]
        for l in labels:
            v = per_launch(l, name)
            flag = ' B' if bimodal(v) else ''
            cells.append(('%.1f/%d%s' % (statistics.median(v), len(v), flag)) if v else '-')
        rows_abs.append(cells)
        aa = ratios(a.aa, name) if a.aa else []
        cells = [name]
        for l in others:
            rs = ratios(l, name)
            if not rs:
                cells.append('-')
                continue
            s = '%.3f [%.2f-%.2f]' % (statistics.median(rs), min(rs), max(rs))
            if len(aa) >= MIN_AA_ROUNDS and l != a.aa:
                s += ' =' if (min(rs) <= max(aa) and max(rs) >= min(aa)) else ' *'
            if bimodal(per_launch(l, name)):
                s += ' B'
            cells.append(s)
        rows_ratio.append(cells)

    print('%d round(s) in %s; labels: %s%s' % (len(rounds), a.dirs, ', '.join(labels),
                                             ('; A/A: %s' % a.aa) if a.aa else ''))
    if a.aa and len(rounds) < MIN_AA_ROUNDS:
        print('fewer than %d rounds: no A/A flags' % MIN_AA_ROUNDS)
    print()
    emit(head_abs, rows_abs, a.md)
    print()
    emit(head_ratio, rows_ratio, a.md)


def emit(head, rows, md):
    if md:
        print('| ' + ' | '.join(head) + ' |')
        print('|---|' + '---:|' * (len(head) - 1))
        for r in rows:
            print('| ' + ' | '.join(r) + ' |')
        return
    w0 = min(60, max(len(r[0]) for r in [head] + rows))
    widths = [max(len(r[i]) for r in [head] + rows) for i in range(1, len(head))]
    print('%-*s' % (w0, head[0][:w0]) + ''.join('  %*s' % (w, h) for w, h in zip(widths, head[1:])))
    for r in rows:
        print('%-*s' % (w0, r[0][:w0]) + ''.join('  %*s' % (w, c) for w, c in zip(widths, r[1:])))


if __name__ == '__main__':
    main()
