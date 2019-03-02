cmake_minimum_required(VERSION 3.13)
project(b-tree)

set(CMAKE_CXX_STANDARD 17)

add_executable(BTree main.cc b_tree.cc b_tree.h)
add_executable(BTreeTest b_tree.cc b_tree.h test.cc)
add_executable(BTreeBench b_tree.h b_tree.cc bench.cc)