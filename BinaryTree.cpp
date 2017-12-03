/*
 * BinaryTree.cpp
 * Hunter Jones
 * COSC 2030, Fall 2017
 * Homework Assignments 4, 5, 6
 * 
 * Implementation of the BinaryTree class.
 */

#include "BinaryTree.h"

#include "MerkleNode.h"
#include <functional>
#include <ostream>
#include <stdexcept>
#include <vector>

BinaryTree::BinaryTree(MerkleNode d): data(d), left(nullptr), right(nullptr)
{
}

BinaryTree::~BinaryTree()
{
    if (left != nullptr) delete left;
    if (right != nullptr) delete right;
}

MerkleNode &BinaryTree::get_data()
{
    return data;
}

const MerkleNode &BinaryTree::see_data() const
{
    return data;
}

inline bool BinaryTree::has_left() const
{
    return left != nullptr;
}

inline bool BinaryTree::has_right() const
{
    return right != nullptr;
}

BinaryTree &BinaryTree::get_left()
{
    if (has_left()) return *left;
    throw std::logic_error("Left does not exist.");
}

BinaryTree &BinaryTree::get_right()
{
    if (has_right()) return *right;
    throw std::logic_error("Right does not exist.");
}

const BinaryTree &BinaryTree::see_left() const
{
    if (has_left()) return *left;
    throw std::logic_error("Left does not exist.");
}

const BinaryTree &BinaryTree::see_right() const
{
    if (has_right()) return *right;
    throw std::logic_error("Right does not exist.");
}

bool BinaryTree::is_leaf() const
{
    return left == nullptr && right == nullptr;
}

unsigned int BinaryTree::size() const
{
    return 1 + (left == nullptr ? 0 : left->size())
             + (right == nullptr ? 0 : right->size());
}

// Private recursive insertion
void BinaryTree::insert(std::vector<BinaryTree *> &vec, const MerkleNode &element)
{
    if (!has_left())
    {
        left = new BinaryTree(element);
        vec.push_back(left);
    }
    else if (!has_right())
    {
        right = new BinaryTree(element);
        vec.push_back(right);
    }
    else if (left->size() > right->size())
    {
        vec.push_back(right);
        right->insert(vec, element);
    }
    else
    {
        vec.push_back(left);
        left->insert(vec, element);
    }
}

// Public insertion
std::vector<BinaryTree *> BinaryTree::insert(const MerkleNode &element)
{
    std::vector<BinaryTree *> vec(1, this);
    insert(vec, element);
    return vec;
}

// Private recursive location
void BinaryTree::locate(unsigned int *steps,
                        std::vector<const BinaryTree *> &vec,
                        const FindPredicate &predicate) const
{
    /* This is a preorder traversal (root, left, right) */
    if (predicate(see_data()))
    {
        return;
    }
    else if (is_leaf())
    {
        throw std::domain_error("Not found.");
    }
    
    try
    {
        if (steps != nullptr) *steps += 1;
        vec.push_back(left);
        left->locate(steps, vec, predicate);
    }
    catch (const std::domain_error &le)
    {
        vec.pop_back();
        if (right != nullptr)
        {
            try
            {
                if (steps != nullptr) *steps += 1;
                vec.push_back(right);
                right->locate(steps, vec, predicate);
            }
            catch (const std::domain_error &re)
            {
                vec.pop_back();
                throw re;
            }
        }
        else
        {
            throw le;
        }
    }
}

// Public location
std::vector<const BinaryTree *> BinaryTree::locate(unsigned int *steps,
                                                   const FindPredicate &predicate) const
{
    std::vector<const BinaryTree *> vec;
    vec.push_back(this);
    if (steps != nullptr) *steps += 1;
    try
    {
        locate(steps, vec, predicate);
        return vec;
    }
    catch (...)
    {
        vec.clear();
        return vec;
    }
}

bool BinaryTree::operator ==(const BinaryTree &other) const
{
    if (data != other.data)
    {
        return false;
    }
    else if (is_leaf() && other.is_leaf())
    {
        return true;
    }
    else
    {
        return *left == other.see_left() &&
               (right != nullptr && other.has_right() ? *right == other.see_right() :
               (right == nullptr && !other.has_right()));
    }
}

bool BinaryTree::operator !=(const BinaryTree &other) const
{
    return !(*this == other);
}

std::ostream &operator <<(std::ostream &out, const BinaryTree &tree)
{
    /* This is a pre-order "list-style" traversal */
    std::function<void(int, const BinaryTree &)> print_util =
            [&print_util, &out](int level, const BinaryTree &now) -> void
    {
        out << std::string(4 * level, ' ') << now.see_data() << "\n";
        if (now.has_left())
        {
            print_util(++level, now.see_left());
            level--;
        }
        if (now.has_right())
        {
            print_util(++level, now.see_right());
            level--;
        }
    };
    
    int level = 0;
    print_util(level, tree);
    
    return out;
}
