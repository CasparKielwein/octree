#ifndef FSO_OCTREE_LIBRARY_H
#define FSO_OCTREE_LIBRARY_H

#include <array>
#include <memory>
#include <vector>

namespace fso {

    struct vec3d{
        float x = 0;
        float y = 0;
        float z = 0;
    };

    inline vec3d operator+(const vec3d& lhs, const vec3d& rhs) {
        return vec3d{lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z};
    }

    inline vec3d operator-(const vec3d& lhs, const vec3d& rhs) {
        return vec3d{lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z};
    }


//alternativly get position through array of objects.
struct Value {
    int v;
    vec3d position;
};


namespace internal {

class Node {
public:
    Node(vec3d c) noexcept : center(c) {}
    ~Node() = default;

    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;

    void insert(Value v);

    Node& subNodeAt(const vec3d& position);

    ///basically jut named indices
    enum Octants {
        XP_YP_ZP = 0,
        XP_YP_ZM,
        XP_YM_ZP,
        XP_YM_ZM,
        XM_YP_ZP,
        XM_YP_ZM,
        XM_YM_ZP,
        XM_YM_ZM,
    };

    auto size() const {
        return content.size();
    }

    auto begin() {
        return content.begin();
    }

    auto end() {
        return content.end();
    }

    /**
     * @brief creates subnodes and moves content to them.
     *
     * Creates all eight subnodes, even if they would be empty.
     * This avoids having to keep a separate leaf flag to track
     * if we need to put new values in this node or create a child.
     *
     * @pre this is a leaf node of the tree.
     */
    void split();

    //private:
    std::array<std::unique_ptr<Node>,8 > children = {};
    std::vector<Value> content = {};

    vec3d center;
};

}


/**
     * Octree to store positions for collision detection
     *
     * Assumptions:
     * * There are only very few objects outside a 100.000^3 box
     * only this box is covered by the octree.
     * * Small collision objects (fighters etc) have a length of ~25
     */
class Octree {
public:

    static constexpr size_t maxUnsplit = 10;

    struct iterator{
        iterator(internal::Node* n, const iterator* p, size_t c) : val(n), parent(p), nr(c) {
        }

        bool operator==(const iterator& other) const {
            return val == other.val;
        }

        internal::Node& operator*() {
            return *val;
        }

        const internal::Node& operator*() const {
            return *val;
        }

        internal::Node* operator->() {
            return val;
        }

        const internal::Node* operator->() const {
            return val;
        }

        /**
         * Implements a depth search of the tree
         *
         * depth search atm, because it is simpler to implement.
         * @return the next iterator
         */
        iterator operator++(int) const{
            return getNext(nr);
        }

        iterator operator++() const {
            return getNext(nr);
        }

    private:
        /// nullptr is used to mark end iterator
        internal::Node* val;
        const iterator* parent;
        size_t nr;

        iterator getNext(size_t current_count) const {
            //first check if there are more children not visited
            for(size_t i = current_count; i < val->children.size(); ++i)
                if (val->children[i] != nullptr)
                    return iterator{val->children[i].get(), this, i};
            //check if we are already at root node, then next is end
            if (parent == nullptr)
                return iterator{nullptr, nullptr, 0};
            else //if not go back to parent
                return parent->getNext(nr);
        }
    };

    Octree();

    iterator begin() {
        return iterator{&root, nullptr, 0};
    }

    iterator end() {
        return iterator(nullptr, nullptr, 0);
    }

    void insert(Value val);

private:
    internal::Node& nodeAt(const vec3d& position);

    /**
     * maximum depth of the tree, root node counts as 1
     *
     * max depth of the tree is set to make the smallest voxel usually contain
     * the full bounding box of a small craft.
     *
     * log2(100.000) + 16.6
     * 64 * 2^11 == 1310722
     *
     */
    static const int maxDepth = 11;

    internal::Node root;
};

}

#endif
