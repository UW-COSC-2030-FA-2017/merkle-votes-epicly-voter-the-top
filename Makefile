all: vote

vote: MerkleTree.cpp vote.cpp
	g++ -O2 MerkleNode.cpp BinaryTree.cpp MerkleTree.cpp vote.cpp -o vote
