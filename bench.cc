//
// Created by xiebei on 2019-03-02.
//
#include "b_tree.h"
#include <iostream>
#include <map>

#include <ctime>
using namespace std;

using RBTree = map<int, string>;


void benchInsert() {
    BTree<int, string> btree;
    RBTree rbtree;

    printf("insert 1000*1000 pairs\n");
    clock_t begin = clock();
    for (int i = 0; i < 1000 * 1000; i++) {
        auto str = std::to_string(i);
        rbtree.insert(std::pair(i, str));
    }
    clock_t end = clock();
    printf("\tRBtree, time used: %.4fsecond\n", (end - begin) / 1000.0 / 1000.0);

    begin = clock();
    for (int i = 0; i < 1000 * 1000; i++) {
        auto str = std::to_string(i);
        btree.insert(i, str);
    }
    end = clock();
    printf("\tBTree, time used: %.4fsecond\n\n", (end - begin) / 1000.0 / 1000.0);
}

void benchSearch() {
    BTree<int, string> btree;
    RBTree rbtree;

    for (int i = 0; i < 1000 * 1000; i++) {
        auto str = std::to_string(i);
        rbtree.insert(std::pair(i, str));
        btree.insert(i, str);
    }

    printf("search 1000*1000 pairs\n");
    clock_t begin = clock();
    for (int i = 0; i < 1000 * 1000; i++) {
        rbtree.find(i);
    }
    clock_t end = clock();
    printf("\tRBtree, time used: %.4fsecond\n", (end - begin) / 1000.0 / 1000.0);

    begin = clock();
    for (int i = 0; i < 1000 * 1000; i++) {
        btree.search(i);
    }
    end = clock();
    printf("\tBTree, time used: %.4fsecond\n\n", (end - begin) / 1000.0 / 1000.0);
}

void benchLargeData() {
    BTree<int, string> btree;
    RBTree rbtree;

    //1KB
    auto largeStr = string(1024 * 4 , 'a');

    printf("large data insert and search 1000*1000 pairs\n");
    clock_t begin = clock();
    for (int i = 0; i < 1000 * 1000; i++) {
        rbtree.insert(std::pair(i, largeStr));
//        rbtree.find(i);
    }
    clock_t end = clock();
    printf("\tRBtree, time used: %.4fsecond\n", (end - begin) / 1000.0 / 1000.0);

    begin = clock();
    for (int i = 0; i < 1000 * 1000; i++) {
        btree.insert(i, largeStr);
//        btree.search(i);
    }
    end = clock();
    printf("\tBTree, time used: %.4fsecond\n\n", (end - begin) / 1000.0 / 1000.0);
}

int main() {
    benchInsert();
    benchSearch();
    benchLargeData();
}