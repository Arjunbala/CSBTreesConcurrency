#include <stdio.h>
#include <iostream>
#include <stdint.h>

using namespace std;

/**
 * CSB Tree Base Class
 */
class CSBTree
{
    private:
    	uint64_t *data; // Assume 64-bit integer primary key index
    	bool ifLeaf; // Indicates if this node is a leaf node
    	CSBTree *left_child; // Pointer to leftmost child
    	int n; // Number of values that can be held by this node

    public:
	/**
	 * Init a new CSB tree with n values possible within each node
	 */
	CSBTree(int num)
	{
	    n = num;
	}

	/**
	 * Insert a new key index into the CSB tree
	 * Needs to be implemented in derived classes
	 */
	virtual int insert(uint64_t key) = 0;

	/**
	 * Search for a given key
	 */
	int search(uint64_t key);

	/**
	 * Delete a key index from the CSB tree
	 */
	int del(uint64_t key);

	/**
	 * Print the CSB tree
	 */
	void print()
	{
	    cout<<"Printing!"<<endl;
	}
};
