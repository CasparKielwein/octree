#include <cassert>
#include "library.h"

namespace fso {

void Octree::insert(Value val) {
    auto &node = nodeAt(val.position);

    if (node.size() < Octree::maxUnsplit)
        node.insert(val);
    else {
        node.split();
        node.subNodeAt(val.position).insert(val);
    }
}

internal::Node &Octree::nodeAt(const vec3d &position) {

    return root.subNodeAt(position);
}

Octree::Octree() : root(vec3d{0,0,0}){

}

void internal::Node::insert(Value v) {
    content.push_back(v);
}

internal::Node &internal::Node::subNodeAt(const vec3d &position) {

    auto offset = position - center;

    auto return_or_this = [this, position](auto *x) -> Node & {
        if (x != nullptr)
            return x->subNodeAt(position);
        else
            return *this;
    };

    if (offset.x < 0) {
        if (offset.y < 0) {
            if (offset.z < 0) {
                return return_or_this(children[XM_YM_ZM].get());
            } else {
                return return_or_this(children[XM_YM_ZP].get());
            }
        } else {
            if (offset.z < 0) {
                return return_or_this(children[XM_YP_ZM].get());
            } else {
                return return_or_this(children[XM_YP_ZP].get());
            }
        }
    } else {
        if (offset.y < 0) {
            if (offset.z < 0) {
                return return_or_this(children[XP_YM_ZM].get());
            } else {
                return return_or_this(children[XP_YM_ZP].get());
            }
        } else {
            if (offset.z < 0) {
                return return_or_this(children[XP_YP_ZM].get());
            } else {
                return return_or_this(children[XP_YP_ZP].get());
            }
        }
    }
}

void internal::Node::split() {
    //create new nodes
    for( auto& child : children) {
        //if the child is already present, something went wrong
        assert(child == nullptr && "called node.split on internal node.");
        //Todo get correct position
        child = std::make_unique<Node>(vec3d{0,0,0});
    }
    //then move content into children
    for (auto& v : content) {
        auto& child = subNodeAt(v.position);
        child.content.push_back(v);
    }
    //todo this will fail, when I model size of objects and objects thus have to stay in internal nodes
    content.clear();
}

}
