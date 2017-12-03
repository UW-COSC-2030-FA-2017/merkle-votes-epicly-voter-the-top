/*
 * MerkleNode.h
 * Hunter Jones
 * COSC 2030, Fall 2017
 * Homework Assignments 4, 5, 6
 * 
 * Interface for the MerkleNode struct.
 */

#pragma once

#include <ostream>
#include <string>

struct MerkleNode
{
    unsigned int timestamp;
    std::string vote_hash;
    
    bool operator ==(const MerkleNode &) const;
    bool operator !=(const MerkleNode &) const;
};

std::ostream &operator <<(std::ostream &, const MerkleNode &);
