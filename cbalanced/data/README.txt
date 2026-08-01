c-balanced Latin squares — one file per n, labelled by strongest guarantee
=========================================================================

Layout of every file
    rows    = items  1..n
    columns = days   1..n
    entry   = the player who receives that item on that day
Every file is a Latin square: rows and columns are both permutations.

Filename encodes the STRONGEST guarantee the square actually satisfies,
re-checked from the grid itself (a square exported at c=2 that happens to
satisfy c=1 is labelled c1):

    nNNN_strict.csv    fully balanced:  Z^t[j] <= ceil(j*n/t)
    nNNN_c1.csv        1-balanced:      Z^t[j] <= floor(j*n/t) + 1
    nNNN_c2.csv        2-balanced:      Z^t[j] <= floor((j+1)*n/t) + 1

strict implies c=1: where j*n/t is an integer the strict radius is j*n/t and
the weak one is j*n/t + 1; elsewhere they coincide. So strict is never weaker.

Contents (226 files, n = 4..229, no gaps)
    strict   10   n = 4 5 6 7 8 9 10 11 13 17
    c=1       6   n = 12 14 15 16 18 20
    c=2     210   n = 19, 21..229

Why the split falls where it does: the paper's impossibility results for full
balance cover n = 12, 6k (k>=2), 6k+2 (k>=3), 6k+4 (k>=4) — all even. Odd n
above 11 are not excluded, and strict squares were indeed found for n = 13 and
n = 17. A c=1 label is an upper bound on what was FOUND, not a proof that no
stronger square exists; for n = 15, 19, 21, 23, 25, 27 a strict search was run
and came up empty within its budget, which is not an impossibility proof.

manifest.json lists every file with n, c, mode and latin, for the viewer.
