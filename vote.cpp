/*
 * vote.cpp
 * Hunter Jones
 * COSC 2030, Fall 2017
 * Homework Assignments 4, 5, 6
 * 
 * Primary driver of this project.
 */

#include "MerkleNode.h"
#include "MerkleTree.h"
#include <cstring>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

// Forward declaration of helper function
void ballots_from_file(std::ifstream &file, std::vector<MerkleNode> &ballots);

int main(int argc, char *argv[])
{
    // Part 1: Argument parsing
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " filename_1 [filename_2] [-h1 / -h2 / -h3]" << std::endl;
        return 1;
    }
    
    unsigned int hash_algorithm = 3;
    std::string filename_1;
    std::string filename_2;
    for (auto i = 1; i < argc; i++)
    {
        if (!std::strncmp(argv[i], "-h", 2))
        {
            hash_algorithm = (unsigned int) std::atoi(&argv[i][2]);
            if (hash_algorithm < 1 || hash_algorithm > 3)
            {
                std::cerr << "Hash algorithm must be 1, 2, or 3." << std::endl;
                return 1;
            }
        }
        else if (filename_1.empty())
        {
            filename_1.assign(argv[i]);
        }
        else if (filename_2.empty())
        {
            filename_2.assign(argv[i]);
        }
        else
        {
            break;
        }
    }
    
    if (filename_1.empty())
    {
        std::cerr << "Please provide at least one filename." << std::endl;
        return 1;
    }
    
    // Part 2: Merkle tree 1
    std::vector<MerkleNode> ballots_1;
    
    {
        std::ifstream file_1(filename_1);
        try
        {
            ballots_from_file(file_1, ballots_1);
        }
        catch (const std::runtime_error &e)
        {
            std::cout << e.what() << ": `" << filename_1 << "`" << std::endl;
            if (e.what()[1] == 'i') // hacky but it works
            {
                std::cout << "Ensure that each line is either blank or in the format of VOTE<tab>TIMESTAMP" << std::endl;
            }
            return 1;
        }
    }
    
    MerkleTree mt_1(hash_algorithm);
    
    for (auto ballot : ballots_1)
    {
        mt_1.insert(ballot);
    }
    
    // Part 3 (optional): Merkle tree 2
    if (!filename_2.empty())
    {
        std::vector<MerkleNode> ballots_2;
        
        {
            std::ifstream file_2(filename_2);
            try
            {
                ballots_from_file(file_2, ballots_2);
            }
            catch (const std::runtime_error &e)
            {
                std::cout << e.what() << ": `" << filename_2 << "`" << std::endl;
                if (e.what()[1] == 'i')
                {
                    std::cout << "Ensure that each line is either blank or in the format of VOTE<tab>TIMESTAMP" << std::endl;
                }
                return 1;
            }
        }
        
        MerkleTree mt_2(hash_algorithm);
        
        for (auto ballot : ballots_2)
        {
            mt_2.insert(ballot);
        }
        
        if (mt_1 == mt_2)
        {
            std::cout << "Validated" << std::endl;
        }
        else
        {
            std::cout << "Not Validated" << std::endl;
        }
    }
    else
    {
        mt_1.print_root(std::cout);
        std::cout << std::endl;
    }
    
    return 0;
}

// Helps read ballots from the specified file format
void ballots_from_file(std::ifstream &file, std::vector<MerkleNode> &ballots)
{
    if (!file.is_open() || file.fail())
    {
        throw std::runtime_error("Failed to open file");
    }
    
    std::string line;
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string seg;
        std::vector<std::string> segs;
        while (std::getline(ss, seg, '\t'))
        {
            segs.push_back(seg);
        }
        
        if (segs.size() != 0)
        {
            if (segs.size() != 2)
            {
                throw std::runtime_error("File is formatted incorrectly");
            }
            ballots.push_back(MerkleNode { (unsigned int) std::stoi(segs[1]), segs[0] });
        }
    }
}
