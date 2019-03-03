//
// Created by xiebei on 2019-03-02.
//

#include <cassert>
#include <string>
#include <map>
#include <iostream>
#include "b_tree.h"

using RBTree = std::map<int, std::string>;

void testRange() {
    BTree<int, std::string> btree;
    RBTree rbtree;
    for (int i = 0; i < 1000 * 1000; i++) {
        auto str = std::to_string(i);
        rbtree.insert(std::pair(i, str));
        btree.insert(i, str);
        assert(btree.search(i).value_or("") == rbtree[i]);
        assert(btree.search(i+1) == std::nullopt);
    }

    for (int i = 2000 * 1000; i > 1000 * 1000; --i) {
        auto str = std::to_string(i);
        rbtree.insert(std::pair(i, str));
        btree.insert(i, str);
        assert(btree.search(i).value_or("") == rbtree[i]);
        assert(btree.search(i-1) == std::nullopt);
    }
}

void testRandom() {
    BTree<int, std::string> btree;
    RBTree rbtree;
    for (int i = 0; i < 1000 * 1000; i++) {
        auto x = static_cast<u32>(rand());
        auto str = std::to_string(x);

        // pass duplicate key
        if (btree.search(x)) {
            continue;
        }

        rbtree.insert(std::pair(x, str));
        btree.insert(x, str);
        assert(btree.search(x).value_or("") == rbtree[x]);
    }
}

int main() {
    testRange();
    testRandom();
}