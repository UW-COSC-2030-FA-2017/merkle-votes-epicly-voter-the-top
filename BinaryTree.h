/*
 * BinaryTree.h
 * Hunter Jones
 * COSC 2030, Fall 2017
 * Homework Assignments 4, 5, 6
 * 
 * Interface for the BinaryTree class.
 */
 
#pragma once

#include "MerkleNode.h"
#include <functional>
#include <ostream>
#include <stdexcept>
#include <vector>

class BinaryTree
{
    // A binary tree is one node and up to two subtrees
    MerkleNode data;
    BinaryTree *left;
    BinaryTree *right;
    
    // Private recursive insertion
    void insert(std::vector<BinaryTree *> &, const MerkleNode &);
    
    typedef std::function<bool(const MerkleNode &)> FindPredicate;
    
    // Private recursive location
    void locate(unsigned int *,
                std::vector<const BinaryTree *> &,
                const FindPredicate &) const;
    
public:
    BinaryTree(MerkleNode);
    
    ~BinaryTree();
    
    MerkleNode &get_data();
    const MerkleNode &see_data() const;
    
    bool has_left() const;
    bool has_right() const;
    
    BinaryTree &get_left();
    BinaryTree &get_right();
    
    const BinaryTree &see_left() const;
    const BinaryTree &see_right() const;
    
    bool is_leaf() const;
    unsigned int size() const;
    
    // Public insertion
    std::vector<BinaryTree *> insert(const MerkleNode &);
    
    // Public location
    std::vector<const BinaryTree *> locate(unsigned int *, const FindPredicate &) const;
    
    bool operator ==(const BinaryTree &) const;
    bool operator !=(const BinaryTree &) const;
};

std::ostream &operator <<(std::ostream &, const BinaryTree &);
