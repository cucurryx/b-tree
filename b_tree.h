//
// Created by xiebei on 2019-03-02.
//

#ifndef BTREE_B_TREE_H
#define BTREE_B_TREE_H

#include <cstdint>
#include <vector>
#include <optional>

// be accustomed to rust
using u32 = u_int32_t;

// pre declaration of BTreeNode
template <typename K, typename V>
class BTreeNode;

// the `t` value which decides how many children a node can have
// the number of children node should be in [t, 2t]
constexpr int kMinmumDegree = 2;

template <typename K, typename V>
class BTree {
public:
    using Key = K;
    using Value = V;
    using BTreeNode = BTreeNode<K, V>;

public:
    void insert(const Key &k, const Value &v);
    std::optional<Value> search(const Key &k);

private:
    std::optional<Value> search(BTreeNode *node, const Key &k);

private:
    BTreeNode *root_;
};

template <typename K, typename V>
std::optional<V> BTree<K, V>::search(const Key &k) {
    return search(root_, k);
}

template <typename K, typename V>
std::optional<V> BTree<K, V>::search(BTreeNode *node, const Key &k) {
    if (node == nullptr) {
        return std::nullopt;
    }
    auto value = node->getValue(k);
    if (value == std::nullopt) {
        if (auto child = node->getChild(k); child != std::nullopt) {
            return search(child, k);
        }
    }
    return value;
}

template <typename K, typename V>
void BTree<K, V>::insert(const Key &k, const Value &v) {
    // TODO
}

template <typename K, typename V>
class BTreeNode {
public:
    using Key = K;
    using Value = V;
    using Self = BTreeNode<K, V>;
    using Pair = std::pair<K, V>;

public:
    BTreeNode() {

    }

    ~BTreeNode() {

    }

    std::optional<Self*> getChild(const Key &k);
    std::optional<Value> getValue(const Key &k);

public:
    bool isLeaf_;
    u32 n_;
    std::vector<Pair> data_;
    std::vector<BTreeNode<K, V>*> children_;
};

//Get the child node by Key k. This function should be called after getValue,
// since there will be wrong if k is in data_ rather than children nodes.
// There are n keys, and n + 1 children. So, the index of child between
// the (i)th key and (i-1)th key is `i`.
template <typename K, typename V>
std::optional<BTreeNode<K, V>*> BTreeNode<K, V>::getChild(const Key &k) {
    if (isLeaf_ or data_.size() == 0) { // leaf doesn't have children
        return std::nullopt;
    }
    assert(data_.size() + 1 == children_.size());
    assert(n_ == data_.size() + 1);

    auto iter = std::find_if(data_.begin(), data_.end(), [&k](const Pair &p){
        return k <= p.first;
    });
    // should call getValue first
    assert(iter->first != k);
    return children_[iter - data_.begin()];
}

// This function is used to find value by key in this BTree Node.
// The return value is `std::optional`. So, if not found, return `std::nullopt`.
template <typename K, typename V>
std::optional<V> BTreeNode<K, V>::getValue(const Key &k) {
    auto iter = std::find(data_.begin(), data_.end(), [&k](const Pair &p){
        return p.first == k;
    });
    return iter == data_.end() ? std::nullopt : iter->second;
}

#endif //BTREE_B_TREE_H
