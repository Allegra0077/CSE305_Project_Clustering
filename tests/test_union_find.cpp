#include "mst/union_find.h"
#include <cassert>
#include <iostream>

int main() {
    UnionFind uf(5);  // 5 elements: 0,1,2,3,4

    // Initially each is its own component
    assert(uf.num_components() == 5);
    for (int i = 0; i < 5; ++i) assert(uf.find(i) == i);
    assert(!uf.same(0, 1));

    // Merge 0 and 1
    assert(uf.unite(0, 1) == true);
    assert(uf.num_components() == 4);
    assert(uf.same(0, 1));
    assert(!uf.same(0, 2));

    // Merge 2 and 3
    assert(uf.unite(2, 3) == true);
    assert(uf.num_components() == 3);

    // Merge {0,1} with {2,3}
    assert(uf.unite(1, 2) == true);
    assert(uf.num_components() == 2);
    assert(uf.same(0, 3));

    // Already same component — should return false
    assert(uf.unite(0, 3) == false);
    assert(uf.num_components() == 2);

    // Merge in element 4
    assert(uf.unite(4, 0) == true);
    assert(uf.num_components() == 1);
    assert(uf.component_size(0) == 5);

    std::cout << "All union-find tests passed!\n";
    return 0;
}