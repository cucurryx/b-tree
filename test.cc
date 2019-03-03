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
    for (int i = 0; i < 10; i++) {
        auto str = std::to_string(i);
        rbtree[i] = str;
        btree.insert(i, str);
        assert(btree.search(i).value_or("") == rbtree[i]);
        assert(btree.search(i+1) == std::nullopt);
    }
}

void testRandom() {

}

int main() {
    testRange();
    testRandom();
}