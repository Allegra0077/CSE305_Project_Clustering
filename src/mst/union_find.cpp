#include "union_find.h"

UnionFind::UnionFind(int n)
    : parent_(n), rank_(n, 0), size_(n, 1), n_components_(n) {
    for (int i = 0; i < n; ++i) parent_[i] = i;
}

int UnionFind::find(int x) {
    // make every node on the path point directly to the root
    while (parent_[x] != x) {
        parent_[x] = parent_[parent_[x]];  // halve the path
        x = parent_[x];
    }
    return x;
}

bool UnionFind::unite(int x, int y) {
    int rx = find(x), ry = find(y);
    if (rx == ry) return false;

    // union by rank: attach the shorter tree under the taller
    if (rank_[rx] < rank_[ry]) std::swap(rx, ry);
    parent_[ry] = rx;
    size_[rx] += size_[ry];
    if (rank_[rx] == rank_[ry]) rank_[rx]++;
    --n_components_;
    return true;
}