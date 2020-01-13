//
// Created by ckielwein on 2020-01-04.
//

#include <cassert>
#include "library.h"

using namespace fso;

void test_initial_insert() {
    Octree tree;

    tree.insert(Value{0,vec3d{1,0,0}});
    tree.insert(Value{0,vec3d{-1,0,0}});

    //expect only the root node to be used for now, as no node has been split yet
    assert(tree.begin()->size() == 2);

    assert(++tree.begin() == tree.end());
}

void test_node_split() {
    Octree tree;

    //add one more than max before split
    for (int i = 0; i <= Octree::maxUnsplit; ++i) {
        tree.insert(Value{0, vec3d{static_cast<float>(i), 0, 0}});
    }

    //expect at least one additional node
    assert(!(++tree.begin() == tree.end()));
}

/// Unit test of internal class Node
void test_node() {
    internal::Node node(vec3d{0,0,0});

    for( auto& x : node.children) {
        assert(x == nullptr);
    }
    assert(node.content.empty());
}

int main() {
    test_node();
    test_initial_insert();
    test_node_split();
}
