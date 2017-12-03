# MerkleVotes

Merkle Is Voting - And it's going to be epic.

## Team members

Hunter Jones

## Building this project

This project uses a modified version of the starter interface, so please use the provided [Makefile](Makefile).

Requires C++11 or later. Tested with GCC 6.3.0 on an ARM processor.

## Running the executable

The makefile will generate an executable called `vote`, which should be used like this:

```
./vote filename_1 [filename_2] [-h1 / -h2 / -h3]
```

Square brackets `[]` denote optional arguments. All arguments can appear in any order. `-h1`, `-h2`, and `-h3` allow you to specify the hashing algorithm to be used, `-h3` is the default.

## Description of hashing algorithms

### hash_1

`hash_1` is an implementation of the DJB2 algorithm by Dan Bernstein. It begins by setting the value of the hash to the prime number 5381. For each character in the input string, the hash is reassigned to the value of the sum of itself shifted left by 5 bits, plus itself, plus the numeric value of the character.

This algorithm has the practical drawback of overflowing for long inputs.

### hash_2

`hash_2` is an implementation of the SDBM algorithm, which was designed for a database library of the same name. Like `hash_1`, this algorithm depends on a magic prime number, in this case 65599. In this algorithm, the hash is set to 0. Then for each character in the input string, the hash is reassigned the value of the sum of itself shifted left by 6 bits, itself shifted left by 16 bits, the negative value of itself, and the numeric value of the character.

The SDBM algorithm is said to have better distribution, scrambling, and boundedness than `hash_1`.

### hash_3

`hash_3` is an implementation of the CRC32 (cyclic redundancy check, 32-bit) algorithm. Here, a the starting value of the hash is 0. Then, the highest 5 bits of the hash are stored for later use. The hash is shifted left by 5 bits, then bitwise XORd with the previously saved highest five bits, bitshifted by 27 bits. The hash is then XOR'd by the numeric value of the current character.

This is probably the best algorithm used in this project (which is the reason it was selected as the default). The reason for this is that this algorithm is designed specifically to accomidate ASCII characters, the most commonly appearing of which have most significance in the last 5 bits. CRC is widely used today because of this.

### Sources

1. [DJB2, SDBM](http://www.cse.yorku.ca/~oz/hash.html)
2. [CRC32](https://www.cs.hmc.edu/~geoff/classes/hmc.cs070.200101/homework10/hashfuncs.html)
