/*
 * MerkleNode.cpp
 * Hunter Jones
 * COSC 2030, Fall 2017
 * Homework Assignments 4, 5, 6
 * 
 * Implementation of the MerkleNode struct.
 */

#include "MerkleNode.h"

#include <ostream>

bool MerkleNode::operator ==(const MerkleNode &other) const
{
    return vote_hash == other.vote_hash;
}

bool MerkleNode::operator !=(const MerkleNode &other) const
{
    return !(*this == other);
}

std::ostream &operator <<(std::ostream &out, const MerkleNode &n)
{
    if (n.timestamp != 0)
    {
        out << "Timestamp: " << n.timestamp << ", ";
    }
    out << (n.timestamp == 0 ? "Hash" : "Vote") << ": " << n.vote_hash;
    return out;
}
