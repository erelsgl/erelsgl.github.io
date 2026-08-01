"""
Builds a self-contained HTML page for inspecting the anchored-window structure
of a c-balanced sequence.

Layout convention (matches the paper's tables):
    rows    = items      1..n   (row r = item r)
    columns = days       1..n   (column t = day t)
    entry   = the player who receives that item on that day

Every column is a permutation of the players, so a t-column window always
contains each player exactly t times.  The c-balance conditions say that for
each t and each j, the ANCHORED window of t columns and
        r_j = floor((j + c - 1) * n / t) + 1
rows is j-full: every player appears in it at least j times.

The page draws those nested windows in shades of blue (darkest for j = 1) and
recomputes the fullness check in the browser from the displayed grid, so the
readout verifies the data rather than trusting it.
"""
from __future__ import annotations
import json
import os
import sys

sys.path.insert(0, os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

from cbalanced import radius            # noqa: E402
from fastbeam import FastBeam
from onepass import greedy_pass         # noqa: E402
from cbal_solve import find_sequence    # noqa: E402
from fastverify import fast_verify      # noqa: E402


def to_grid(days, n):
    """grid[item-1][day-1] = player (1-indexed)."""
    g = [[0] * n for _ in range(n)]
    for t, assign in enumerate(days):
        for i, item in enumerate(assign):
            g[item - 1][t] = i + 1
    return g


def is_latin(grid, n):
    for row in grid:
        if sorted(row) != list(range(1, n + 1)):
            return False
    return True


def build(targets):
    out = []
    for (n, c, note) in targets:
        days, depth = greedy_pass(n, c)
        how = "greedy pass"
        if days is None:
            days, st = find_sequence(n, c, time_limit=45, mode="weak", seed=1)
            how = "search"
        if days is None:
            B = FastBeam(n, c)
            days, st = B.run(width=400, budget=4000, time_limit=90, seed=0, explore=0.0)
            how = "beam search"
        if days is None:
            print(f"  n={n} c={c}: NOT FOUND, skipping")
            continue
        ok, msg = fast_verify(days, n, c)
        if not ok:
            print(f"  n={n} c={c}: verification failed ({msg}), skipping")
            continue
        grid = to_grid(days, n)
        out.append({
            "n": n, "c": c, "note": note, "how": how,
            "grid": grid, "latin": is_latin(grid, n),
            "label": f"n = {n},  c = {c}",
        })
        print(f"  n={n} c={c}: ok via {how}, latin={is_latin(grid,n)}")
    return out


HTML = r"""<!doctype html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>Anchored windows in a c-balanced sequence</title>
<style>
  :root {
    --paper:   #eceff3;
    --panel:   #ffffff;
    --ink:     #0e1922;
    --muted:   #5d6b78;
    --rule:    #c2ccd6;
    --edge:    #163a5c;
    --warn:    #a8431a;
    --ok:      #1f6b45;
    --font-data: ui-monospace, "SF Mono", "Cascadia Mono", "Roboto Mono", Menlo, Consolas, monospace;
    --font-ui: "Inter", system-ui, -apple-system, "Segoe UI", Roboto, sans-serif;
  }
  * { box-sizing: border-box; }
  body {
    margin: 0; background: var(--paper); color: var(--ink);
    font-family: var(--font-ui); line-height: 1.5;
    padding: 28px 24px 64px;
  }
  .wrap { max-width: 1180px; margin: 0 auto; }
  h1 {
    font-family: var(--font-data); font-size: 15px; font-weight: 600;
    letter-spacing: .18em; text-transform: uppercase; margin: 0 0 6px;
  }
  .sub { color: var(--muted); font-size: 14px; margin: 0 0 22px; max-width: 74ch; }
  .sub code { font-family: var(--font-data); font-size: 13px; background: #dfe5ec;
              padding: 1px 5px; border-radius: 3px; }

  .bar {
    display: flex; flex-wrap: wrap; gap: 18px 26px; align-items: center;
    background: var(--panel); border: 1px solid var(--rule); border-radius: 6px;
    padding: 14px 18px; margin-bottom: 8px;
  }
  .bar label { font-family: var(--font-data); font-size: 11px; letter-spacing: .12em;
               text-transform: uppercase; color: var(--muted); display: block; margin-bottom: 4px; }
  select, input[type=range] { font-family: var(--font-data); font-size: 13px; }
  select { padding: 5px 8px; border: 1px solid var(--rule); border-radius: 4px; background: #fff; color: var(--ink); }
  input[type=range] { width: 260px; accent-color: var(--edge); vertical-align: middle; }
  .tval { font-family: var(--font-data); font-size: 20px; font-weight: 600; min-width: 3ch; display: inline-block; }
  .badge { font-family: var(--font-data); font-size: 11px; letter-spacing: .08em;
           border: 1px solid var(--rule); border-radius: 3px; padding: 3px 8px; color: var(--muted); }
  .badge.good { color: var(--ok); border-color: #a9cdbb; }

  .cols { display: grid; grid-template-columns: minmax(0,1fr) 320px; gap: 22px; align-items: start; }
  @media (max-width: 940px) { .cols { grid-template-columns: minmax(0,1fr); } }

  .board { background: var(--panel); border: 1px solid var(--rule); border-radius: 6px;
           padding: 16px; overflow: auto; }
  table.grid { border-collapse: collapse; font-family: var(--font-data); }
  table.grid td, table.grid th { text-align: center; padding: 0; }
  table.grid th {
    font-weight: 500; color: var(--muted); font-size: 10px;
  }
  table.grid th.day { cursor: pointer; }
  table.grid th.day:hover { color: var(--edge); }
  table.grid th.day.sel { color: var(--edge); font-weight: 700; }
  td.cell { border: 1px solid rgba(255,255,255,.55); }
  td.cell.spot { outline: 2px solid var(--warn); outline-offset: -2px; z-index: 2; position: relative; }
  td.cell.outside { background: #f4f6f8; color: #b6c0ca; border-color: #e6eaee; }
  .redge { box-shadow: inset 0 -2px 0 0 var(--edge); }
  .cedge { box-shadow: inset -2px 0 0 0 var(--edge); }
  .redge.cedge { box-shadow: inset 0 -2px 0 0 var(--edge), inset -2px 0 0 0 var(--edge); }

  aside { background: var(--panel); border: 1px solid var(--rule); border-radius: 6px; padding: 16px; }
  aside h2 { font-family: var(--font-data); font-size: 11px; letter-spacing: .14em;
             text-transform: uppercase; color: var(--muted); margin: 0 0 10px; font-weight: 600; }
  table.read { width: 100%; border-collapse: collapse; font-family: var(--font-data); font-size: 12px; }
  table.read th { text-align: left; font-weight: 500; color: var(--muted); font-size: 10px;
                  letter-spacing: .08em; text-transform: uppercase; padding: 0 6px 6px 0; }
  table.read td { padding: 3px 6px 3px 0; border-top: 1px solid #e6eaee; }
  .swatch { display: inline-block; width: 11px; height: 11px; border-radius: 2px;
            vertical-align: -1px; margin-right: 6px; border: 1px solid rgba(0,0,0,.12); }
  .pass { color: var(--ok); }
  .fail { color: var(--warn); font-weight: 700; }
  .stair { margin-top: 16px; }
  .note { font-size: 12px; color: var(--muted); margin-top: 12px; }
  .hint { font-size: 12px; color: var(--muted); margin: 10px 2px 0; }
</style>
</head>
<body>
<div class="wrap">
  <h1>Anchored windows</h1>
  <p class="sub">
    Rows are items, columns are days, each entry is the player who receives that item that day.
    The <em>c</em>-balance conditions all have one form: for each <code>t</code> and each
    <code>j</code>, the anchored window of <code>t</code> columns and
    <code>r_j = floor((j+c-1)&middot;n/t) + 1</code> rows must be <strong>j-full</strong> &mdash;
    every player appears in it at least <code>j</code> times. Pick a <code>t</code> to see those
    nested windows; darkest blue is <code>j = 1</code>.
  </p>

  <div class="bar">
    <div>
      <label for="seq">sequence</label>
      <select id="seq"></select>
    </div>
    <div>
      <label for="t">days t (or click a column header)</label>
      <input type="range" id="t" min="1" value="1">
      <span class="tval" id="tval">1</span>
    </div>
    <div>
      <label for="spot">highlight player</label>
      <select id="spot"></select>
    </div>
    <div id="badges"></div>
  </div>
  <p class="hint" id="hint"></p>

  <div class="cols">
    <div class="board" id="board"></div>
    <aside>
      <h2>Fullness check at this t</h2>
      <table class="read"><thead>
        <tr><th>j</th><th>rows r_j</th><th>need</th><th>min count</th><th></th></tr>
      </thead><tbody id="readout"></tbody></table>
      <div class="stair">
        <h2>Window staircase</h2>
        <svg id="stair" width="288" height="150" role="img" aria-label="r_j as a function of j"></svg>
      </div>
      <p class="note" id="note"></p>
    </aside>
  </div>
</div>

<script>
const DATA = __DATA__;

const $ = id => document.getElementById(id);
let cur = 0, T = 1, spotPlayer = 0;

function rj(n, c, t, j) {
  // floor((j + c - 1) * n / t) + 1, computed exactly for rational c
  const num = (j + c - 1) * n;
  return Math.floor(num / t) + 1;
}

// the j-th window has r_j rows; only j with r_j <= n are visually distinct
function windows(seq, t) {
  const out = [];
  for (let j = 1; j <= t; j++) {
    const r = Math.min(seq.n, rj(seq.n, seq.c, t, j));
    out.push({ j, r, trivial: rj(seq.n, seq.c, t, j) >= seq.n });
    if (r >= seq.n) break;          // beyond this every window is the full height
  }
  return out;
}

// shade index of a cell: the smallest j whose window contains it
function shadeOf(wins, row) {
  for (let k = 0; k < wins.length; k++) if (row <= wins[k].r) return k;
  return -1;
}

function colourFor(k, total) {
  if (k < 0) return null;
  const f = total <= 1 ? 0 : k / (total - 1);
  const light = 32 + f * 55;                 // 32% (j=1, darkest) -> 87%
  const sat   = 62 - f * 18;
  return { bg: `hsl(209 ${sat}% ${light}%)`, fg: light < 58 ? '#f2f6fa' : '#0e1922' };
}

// count, for each player, occurrences in the anchored window t x r
function minCount(seq, t, r) {
  const n = seq.n, tally = new Array(n + 1).fill(0);
  for (let row = 0; row < r; row++)
    for (let col = 0; col < t; col++) tally[seq.grid[row][col]]++;
  let m = Infinity, who = 0;
  for (let p = 1; p <= n; p++) if (tally[p] < m) { m = tally[p]; who = p; }
  return { min: m, who };
}

function render() {
  const seq = DATA[cur], n = seq.n;
  const wins = windows(seq, T);
  const total = wins.length;

  // ---- grid ----
  let h = '<table class="grid"><thead><tr><th></th>';
  for (let c = 1; c <= n; c++)
    h += `<th class="day${c === T ? ' sel' : ''}" data-col="${c}">${c}</th>`;
  h += '</tr></thead><tbody>';
  const size = n > 20 ? 22 : n > 14 ? 26 : 30;
  const fs = n > 20 ? 10 : 12;
  for (let row = 1; row <= n; row++) {
    h += `<tr><th style="padding-right:6px">${row}</th>`;
    for (let col = 1; col <= n; col++) {
      const inside = col <= T;
      const k = inside ? shadeOf(wins, row) : -1;
      const col2 = colourFor(k, total);
      const v = seq.grid[row - 1][col - 1];
      const cls = ['cell'];
      if (!inside || k < 0) cls.push('outside');
      if (spotPlayer && v === spotPlayer) cls.push('spot');
      if (inside && wins.some(w => w.r === row)) cls.push('redge');
      if (inside && col === T) cls.push('cedge');
      const style = col2
        ? `background:${col2.bg};color:${col2.fg};`
        : '';
      h += `<td class="${cls.join(' ')}" style="${style}width:${size}px;height:${size}px;font-size:${fs}px">${v}</td>`;
    }
    h += '</tr>';
  }
  h += '</tbody></table>';
  $('board').innerHTML = h;
  $('board').querySelectorAll('th.day').forEach(el =>
    el.addEventListener('click', () => { T = +el.dataset.col; sync(); }));

  // ---- readout ----
  let r = '';
  let allPass = true;
  wins.forEach((w, k) => {
    const { min, who } = minCount(seq, T, w.r);
    const pass = min >= w.j;
    if (!pass) allPass = false;
    const col2 = colourFor(k, total);
    r += `<tr>
      <td><span class="swatch" style="background:${col2.bg}"></span>${w.j}</td>
      <td>${w.r}${w.trivial ? '<span style="color:var(--muted)"> (all)</span>' : ''}</td>
      <td>&ge; ${w.j}</td>
      <td>${min}${min === w.j ? '' : ''}</td>
      <td class="${pass ? 'pass' : 'fail'}">${pass ? '\u2713' : '\u2717 p' + who}</td>
    </tr>`;
  });
  $('readout').innerHTML = r;

  // ---- staircase ----
  const W = 288, H = 150, pad = 26;
  const maxJ = Math.max(...wins.map(w => w.j)), maxR = seq.n;
  let s = `<rect x="0" y="0" width="${W}" height="${H}" fill="#fbfcfd"/>`;
  s += `<line x1="${pad}" y1="${H - pad}" x2="${W - 6}" y2="${H - pad}" stroke="#c2ccd6"/>`;
  s += `<line x1="${pad}" y1="6" x2="${pad}" y2="${H - pad}" stroke="#c2ccd6"/>`;
  wins.forEach((w, k) => {
    const x0 = pad + (w.j - 1) / Math.max(1, maxJ) * (W - pad - 10);
    const x1 = pad + w.j / Math.max(1, maxJ) * (W - pad - 10);
    const y = (H - pad) - (w.r / maxR) * (H - pad - 10);
    const col2 = colourFor(k, total);
    s += `<rect x="${x0}" y="${y}" width="${Math.max(1, x1 - x0)}" height="${(H - pad) - y}" fill="${col2.bg}" stroke="#fff" stroke-width=".5"/>`;
  });
  s += `<text x="${pad}" y="${H - 8}" font-size="9" fill="#5d6b78" font-family="ui-monospace,monospace">j = 1</text>`;
  s += `<text x="${W - 10}" y="${H - 8}" font-size="9" fill="#5d6b78" text-anchor="end" font-family="ui-monospace,monospace">${maxJ}</text>`;
  s += `<text x="4" y="14" font-size="9" fill="#5d6b78" font-family="ui-monospace,monospace">r=${maxR}</text>`;
  $('stair').innerHTML = s;

  // ---- badges ----
  $('badges').innerHTML =
    `<span class="badge ${allPass ? 'good' : ''}">${allPass ? 'all windows j-full' : 'a window failed'}</span> ` +
    `<span class="badge">${seq.latin ? 'Latin square' : 'not a Latin square'}</span> ` +
    `<span class="badge">found by ${seq.how}</span>`;
  $('note').textContent = seq.latin
    ? 'Every row holds each player once, so this is a Latin square.'
    : 'Rows may repeat a player: weak balance does not force the Latin property. Columns are always permutations.';
  $('hint').textContent =
    `Window j = 1 is the darkest and shortest; each larger j adds rows. Every player must appear at least j times inside window j.`;
}

function sync() {
  const seq = DATA[cur];
  $('t').max = seq.n;
  if (T > seq.n) T = seq.n;
  $('t').value = T;
  $('tval').textContent = T;
  render();
}

function init() {
  $('seq').innerHTML = DATA.map((d, i) =>
    `<option value="${i}">${d.label}${d.note ? '  \u2014 ' + d.note : ''}</option>`).join('');
  $('seq').addEventListener('change', e => {
    cur = +e.target.value; T = Math.min(T, DATA[cur].n);
    spotPlayer = 0; buildSpot(); sync();
  });
  $('t').addEventListener('input', e => { T = +e.target.value; sync(); });
  $('spot').addEventListener('change', e => { spotPlayer = +e.target.value; render(); });
  buildSpot();
  T = Math.max(2, Math.floor(DATA[0].n / 3));
  sync();
}

function buildSpot() {
  const n = DATA[cur].n;
  let o = '<option value="0">none</option>';
  for (let p = 1; p <= n; p++) o += `<option value="${p}">player ${p}</option>`;
  $('spot').innerHTML = o;
}

init();
</script>
</body>
</html>
"""


def main():
    targets = [
        (8,  1, "small warm-up"),
        (12, 1, "largest n with a fully balanced sequence"),
        (13, 1, ""),
        (16, 1, ""),
        (19, 2, "n=19 needs c=2; c=1 is unresolved"),
        (20, 1, ""),
        (24, 2, ""),
    ]
    print("building sequences:")
    seqs = build(targets)
    here = os.path.dirname(os.path.abspath(__file__))
    out = os.path.join(here, "cbalanced_windows.html")
    html = HTML.replace("__DATA__", json.dumps(seqs, separators=(",", ":")))
    with open(out, "w") as f:
        f.write(html)
    print(f"\nwrote {out}  ({len(html)//1024} KB, {len(seqs)} sequences)")


if __name__ == "__main__":
    main()
