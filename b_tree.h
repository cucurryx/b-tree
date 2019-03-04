//
// Created by xiebei on 2019-03-02.
//

#ifndef BTREE_B_TREE_H
#define BTREE_B_TREE_H

#include <cstdint>
#include <vector>
#include <optional>
#include <iostream>

// be accustomed to rust
using u32 = u_int32_t;

// pre declaration of BTreeNode
template <typename K, typename V>
class BTreeNode;

// the `t` value which decides how many children a node can have
// the number of children node should be in [t, 2t]
constexpr int kMinDegree = 4;

template <typename K, typename V>
class BTree {
public:
    using Key = K;
    using Value = V;
    using BTreeNode = BTreeNode<K, V>;
    using Pair = std::pair<K, V>;

public:
    BTree() {
        root_ = new BTreeNode(true, std::vector<Pair>(), std::vector<BTreeNode*>());
    }

    ~BTree() {
        delete root_;
    }

    void insert(const Key &k, const Value &v);
    void erase(const Key &k);
    std::optional<Value> search(const Key &k);

private:
    std::optional<Value> search(BTreeNode *node, const Key &k);
    void insert(BTreeNode *node, const BTree::Pair &p);
    void insertNotFull(BTreeNode *node, const BTree::Pair &p);
    void erase(BTreeNode *node, const Key &k);

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
            return search(child.value_or(nullptr), k);
        }
    }
    return value;
}

template <typename K, typename V>
void BTree<K, V>::erase(const Key &k) {
    return erase(root_, k);
}

template <typename K, typename V>
void BTree<K, V>::erase(BTreeNode *node, const Key &k) {
    if (node == nullptr) {
        //log warning
        return;
    }

    
}

template <typename K, typename V>
void BTree<K, V>::insert(const Key &k, const Value &v) {
    insert(root_, Pair(k ,v));
}

template <typename K, typename V>
void BTree<K, V>::insert(BTreeNode *node, const BTree::Pair &p) {
    if (node == nullptr) {
        //FIXME: panic?
        return;
    }
    if (node->n_ == 2 * kMinDegree - 1) {
        auto *newNode = new BTreeNode(
                false,
                std::vector<Pair>(),
                std::vector<BTreeNode*>{ root_ });
        root_ = newNode;
        newNode->splitChild(0); // split old root
        insertNotFull(newNode, p);
    } else {
        insertNotFull(node, p);
    }
}

template <typename K, typename V>
void BTree<K, V>::insertNotFull(BTreeNode *node, const BTree::Pair &p) {
    if (node == nullptr) {
        //FIXME, log warn?
        return;
    }

    const auto &key = p.first;
    auto iter = std::find_if(node->data_.begin(), node->data_.end(), 
            [&key](const Pair &curr){ return curr.first >= key; });

    if (not node->isLeaf_) { // just insert
        u32 i = iter - node->data_.begin();
        auto *child = node->children_[i];
        if (child->n_ == 2 * kMinDegree - 1) {
            node->splitChild(i);
            if (node->data_[i].first < key) { ++i; }
        }
        insertNotFull(node->children_[i], p);
    } else {
        node->data_.insert(iter, p);
        ++node->n_;
    }
}

template <typename K, typename V>
class BTreeNode {
public:
    using Key = K;
    using Value = V;
    using Self = BTreeNode<K, V>;
    using Pair = std::pair<K, V>;

public:
    BTreeNode() = default;

    BTreeNode(bool leaf, std::vector<Pair> &&data, std::vector<BTreeNode*> &&children) {
        isLeaf_ = leaf;
        data_ = std::move(data);
        children_ = std::move(children);
        n_ = static_cast<u32>(data_.size());
    }

    ~BTreeNode() {
        for (auto child : children_) {
            delete child;
        }
    }

    std::optional<Self*> getChild(const Key &k);
    std::optional<Value> getValue(const Key &k);
    void splitChild(u32 i);

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
    auto iter = std::find_if(data_.begin(), data_.end(), [&k](const Pair &p){
        return k <= p.first;
    });
    return children_[iter - data_.begin()];
}

// This function is used to find value by key in this BTree Node.
// The return value is `std::optional`. So, if not found, return `std::nullopt`.
template <typename K, typename V>
std::optional<V> BTreeNode<K, V>::getValue(const Key &k) {
    auto iter = std::find_if(data_.begin(), data_.end(), [&k](const Pair &p){
        return p.first == k;
    });
    return iter == data_.end() ? std::nullopt : std::optional<V>(iter->second);
}

// Split the i-th child of this node to prevent backtracking when insert node in child.
// This function will split the i-th node into two node and both of them have `t` children and `t-1` keys.
// And then, key in the mid of the n-th child will be brought to this node.
// A lot of operation on vector...
template <typename K, typename V>
void BTreeNode<K, V>::splitChild(u32 i) {
    BTreeNode *oldLeft = children_[i], *newRight = new BTreeNode();
    newRight->isLeaf_ = oldLeft->isLeaf_;

    auto iterEnd = oldLeft->data_.end();
    newRight->data_.insert(newRight->data_.begin(), iterEnd - kMinDegree + 1, iterEnd);
    newRight->n_ += kMinDegree - 1;

    iterEnd = oldLeft->data_.end();
    oldLeft->data_.erase(iterEnd - kMinDegree + 1, iterEnd);
    oldLeft->n_ -= kMinDegree;

    data_.insert(data_.begin() + i, *(oldLeft->data_.end() - 1));
    children_.insert(children_.begin() + i + 1, newRight);
    ++n_;

    oldLeft->data_.pop_back();
    if (not oldLeft->isLeaf_) {
        newRight->children_.insert(
                newRight->children_.begin(),
                oldLeft->children_.end() - kMinDegree,
                oldLeft->children_.end());

        oldLeft->children_.erase(
                oldLeft->children_.end() - kMinDegree,
                oldLeft->children_.end());
    }
}

#endif //BTREE_B_TREE_H
