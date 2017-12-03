/*
 * MerkleTree.h
 * Hunter Jones
 * COSC 2030, Fall 2017
 * Homework Assignments 4, 5, 6
 * 
 * Interface for the MerkleTree class.
 */

#pragma once

#include "BinaryTree.h"
#include <ostream>
#include <string>
#include <vector>

class MerkleTree
{
    BinaryTree tree;
    bool empty;
    
    static std::string uint_to_hex_str(const unsigned int &);
    
    std::string (*hash)(const std::string &);
    
    static std::string hash_1(const std::string &);
    static std::string hash_2(const std::string &);
    static std::string hash_3(const std::string &);
    
public:
    MerkleTree(unsigned int);
    MerkleTree(unsigned int, const BinaryTree &);
    ~MerkleTree();
    
    unsigned int insert(const MerkleNode &);
    
    unsigned int find_vote(const MerkleNode &) const;
    unsigned int find_hash(const MerkleNode &) const;
    
    friend bool operator ==(const MerkleTree&, const MerkleTree &);
    friend bool operator !=(const MerkleTree&, const MerkleTree &);
    
    friend std::ostream &operator <<(std::ostream &, const MerkleTree &);
    void print_root(std::ostream &);
};
