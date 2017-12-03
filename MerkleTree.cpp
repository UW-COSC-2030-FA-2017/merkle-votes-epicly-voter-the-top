/*
 * MerkleTree.cpp
 * Hunter Jones
 * COSC 2030, Fall 2017
 * Homework Assignments 4, 5, 6
 * 
 * Implementation of the MerkleTree class.
 */

#include "MerkleTree.h"

#include "BinaryTree.h"
#include <ios>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>

std::string MerkleTree::uint_to_hex_str(const unsigned int &i)
{
    std::stringstream ss;
    ss << std::hex << i;
    return ss.str();
}

std::string MerkleTree::hash_1(const std::string &input)
{
    // http://www.cse.yorku.ca/~oz/hash.html
    /* An implementation of the DJB2 algorithm */
    unsigned int hash = 5381;
    for (unsigned char c : input)
    {
        hash = ((hash << 5) + hash) + c;
    }
    return uint_to_hex_str(hash);
}

std::string MerkleTree::hash_2(const std::string &input)
{
    // http://www.cse.yorku.ca/~oz/hash.html
    /* An implementation of the SDBM algorithm */
    unsigned int hash = 0;
    for (unsigned char c : input)
    {
        hash = (hash << 6) + (hash << 16) - hash + c;
    }
    return uint_to_hex_str(hash);
}

std::string MerkleTree::hash_3(const std::string &input)
{
    // https://www.cs.hmc.edu/~geoff/classes/hmc.cs070.200101/homework10/hashfuncs.html
    /* An implementation of the CRC algorithm */
    unsigned int hash = 0;
    for (unsigned char c : input)
    {
        auto high_five = hash & 0xF8000000;
        hash <<= 5;
        hash ^= high_five >> 27;
        hash ^= c;
    }
    return uint_to_hex_str(hash);
}

MerkleTree::MerkleTree(unsigned int algorithm): tree(MerkleNode { 0, "" }), empty(true)
{
    if (algorithm < 1 || algorithm > 3)
    {
        throw std::out_of_range("Algorithm must be 1, 2, or 3.");
    }
    switch (algorithm)
    {
    case 1:
        hash = &MerkleTree::hash_1;
        break;
    case 2:
        hash = &MerkleTree::hash_2;
        break;
    default:
        hash = &MerkleTree::hash_3;
    }
}

MerkleTree::MerkleTree(unsigned int algorithm, const BinaryTree &from): MerkleTree(algorithm)
{
    empty = false;
    tree = from;
}

MerkleTree::~MerkleTree()
{
}

unsigned int MerkleTree::insert(const MerkleNode &vote)
{
    if (empty)
    {
        tree.get_data() = vote;
        empty = false;
        return 1;
    }
    else
    {
        // Insert the vote
        auto path = tree.insert(vote);
        
        // Swap the parent node of the inserted node with a new hash node
        auto parent = path.at(path.size() - 2);
        parent->insert(path.back()->see_data());
        path.back()->get_data() = parent->get_data();
        parent->get_data().timestamp = 0;
        parent->get_data().vote_hash = (*hash)(parent->see_left().see_data().vote_hash +
                                                parent->see_right().see_data().vote_hash);
        
        // Recompute hashes up to the root of the tree
        unsigned int recompute_ops = 0;
        for (auto itr = path.rbegin() + 2; itr != path.rend(); itr++)
        {
            recompute_ops++;
            (*itr)->get_data().vote_hash = (*hash)((*itr)->see_left().see_data().vote_hash +
                                           (*itr)->see_right().see_data().vote_hash);
        }
        
        // Number of operations == size of path + 1 for parent swap + however many hash recomputations were needed
        return path.size() + 1 + recompute_ops;
    }
}

unsigned int MerkleTree::find_vote(const MerkleNode &vote) const
{
    if (empty) return 0;
    unsigned int ops = 0;
    try
    {
        tree.locate(&ops, [&vote](const BinaryTree &t) -> bool
        {
            return t.is_leaf() && t.see_data().timestamp == vote.timestamp
                               && t.see_data().vote_hash == vote.vote_hash;
        });
    }
    catch (...)
    {
        ops = 0;
    }
    return ops;
}

unsigned int MerkleTree::find_hash(const MerkleNode &hash) const
{
    if (empty) return 0;
    unsigned int ops = 0;
    try
    {
        tree.locate(&ops, [&hash](const BinaryTree &t) -> bool
        {
            return !t.is_leaf() && t.see_data().vote_hash == hash.vote_hash;
        });
    }
    catch (...)
    {
        ops = 0;
    }
    return ops;
}

bool operator ==(const MerkleTree &lhs, const MerkleTree &rhs)
{
    return lhs.tree.see_data() == rhs.tree.see_data();
}

bool operator !=(const MerkleTree &lhs, const MerkleTree &rhs)
{
    return !(lhs == rhs);
}

std::ostream &operator <<(std::ostream &out, const MerkleTree &tree)
{
    out << tree.tree;
    return out;
}

void MerkleTree::print_root(std::ostream &out)
{
    out << tree.see_data();
}
