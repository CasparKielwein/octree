//
// Created by ckielwein on 2020-01-04.
//

#include <cassert>
#include "library.h"

using namespace fso;

void test_initial_insert() {
    Octree tree;

    tree.insert(Octree::Value{0,vec3d{1,0,0}});
    tree.insert(Octree::Value{0,vec3d{-1,0,0}});

    //expect only the root node to be used for now, as no node has been split yet
    assert(tree.begin()->size() == 2);

    assert(++tree.begin() == tree.end());
}

void test_node_split() {
    Octree tree;

    for(int i = 0; i != Octree::maxUnsplit; ++i) {
        tree.insert(Octree::Value{0,vec3d{static_cast<float>(i),0,0}});
    }

    //expect at least one additional node
    assert(!(++tree.begin() == tree.end()));
}

int main() {
    test_initial_insert();
    test_node_split();
}
