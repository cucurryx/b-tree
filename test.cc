//
// Created by xiebei on 2019-03-02.
//

#include <cassert>
#include "b_tree.h"

void testRange() {

}

void testRandom() {
    BTree<int, int> tree;
    auto result = tree.search(1).value_or(0);
}

int main() {
    testRange();
    testRandom();
}