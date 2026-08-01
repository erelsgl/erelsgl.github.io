"""
Export c-balanced Latin squares as CSV.

One file per sequence, written to viz/data/:

    # n=12
    # c=1
    # latin=true
    # rows=items, cols=days, entry=player
    7,3,11,...
    ...

Metadata lives in leading '#' lines so the viewer can read n and c without
relying on the filename. A manifest.json is written alongside so the viewer can
auto-load the set when the folder is served over HTTP.
"""
from __future__ import annotations
import json
import os
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, os.path.dirname(HERE))

from latin import LatinSolver, verify_latin   # noqa: E402


def to_grid(days, n):
    """grid[item-1][day-1] = player."""
    g = [[0] * n for _ in range(n)]
    for t, assign in enumerate(days):
        for i, item in enumerate(assign):
            g[item - 1][t] = i + 1
    return g


def find(n, c, mode="weak", time_limit=30.0):
    S = LatinSolver(n, c, mode=mode, latin=True)
    if not S.ok:
        return None, "propagation"
    d, dep = S.greedy()
    if d is not None:
        return d, "greedy"
    for br in (6, 8, 16):
        d, st = S.search(branch=br, node_limit=300000, seed=0,
                         time_limit=time_limit)
        if d is not None:
            return d, f"search(branch={br})"
    return None, "not found"


def write_csv(path, grid, n, c, mode, how):
    with open(path, "w") as f:
        f.write("# c-balanced Latin square\n")
        f.write(f"# n={n}\n")
        f.write(f"# c={c}\n")
        f.write(f"# mode={mode}\n")
        f.write("# latin=true\n")
        f.write(f"# found_by={how}\n")
        f.write("# rows=items, cols=days, entry=player\n")
        for row in grid:
            f.write(",".join(str(x) for x in row) + "\n")


def main():
    out_dir = os.path.join(HERE, "data")
    os.makedirs(out_dir, exist_ok=True)

    import argparse
    ap = argparse.ArgumentParser()
    ap.add_argument("--c2-max", type=int, default=40,
                    help="largest n to export at c=2 (e.g. 229)")
    ap.add_argument("--quiet", action="store_true")
    args = ap.parse_args()

    targets = []
    for n in list(range(4, 19)) + [20]:
        targets.append((n, 1, "weak"))
    for n in range(4, args.c2_max + 1):
        targets.append((n, 2, "weak"))
    for n in [8, 10, 11]:
        targets.append((n, 1, "strict"))

    manifest = []
    for (n, c, mode) in targets:
        days, how = find(n, c, mode)
        if days is None:
            print(f"  n={n:3d} c={c} {mode:6s}: {how}, skipped")
            continue
        ok, msg = verify_latin(days, n, c, mode)
        if not ok:
            print(f"  n={n:3d} c={c} {mode:6s}: FAILED verification ({msg})")
            continue
        tag = "" if mode == "weak" else "_strict"
        name = f"latin_n{n:03d}_c{c}{tag}.csv"
        write_csv(os.path.join(out_dir, name), to_grid(days, n), n, c, mode, how)
        manifest.append({"file": name, "n": n, "c": c, "mode": mode})
        if not args.quiet:
            print(f"  n={n:3d} c={c} {mode:6s}: ok ({how}) -> {name}")

    with open(os.path.join(out_dir, "manifest.json"), "w") as f:
        json.dump(manifest, f, indent=1)
    print(f"\nwrote {len(manifest)} CSV files + manifest.json to {out_dir}")


if __name__ == "__main__":
    main()
