#pragma once
#include <vector>

class UnionFind {
public:
    explicit UnionFind(int n);

    // returns the representative (root) of x's component
    int find(int x);

    // merges components containing x and y
    // returns true if they were in different components (a merge happened)
    // false if they were already in the same component
    bool unite(int x, int y);

    // true iff x and y are in the same component
    bool same(int x, int y) { return find(x) == find(y); }

    // num of disjoint components currently
    int num_components() const { return n_components_; }

    // size of the component containing x
    int component_size(int x) { return size_[find(x)]; }

private:
    std::vector<int> parent_;
    std::vector<int> rank_;
    std::vector<int> size_;
    int n_components_;
};