#include <stdio.h>
#include <iostream>
#include <stdint.h>

using namespace std;

/**
 * Representation of a CSB Tree node
 */
class CSBNode
{
    private:
        uint64_t *data; // Assume 64-bit integer primary key index
        bool ifLeaf; // Indicates if this node is a leaf node
        CSBNode *left_child; // Pointer to leftmost child
        int nKeys; // Number of keys currently held by this node

    public:

	/**
	 * Init a new CSB node given the order of the CSB tree
	 */
	CSBNode(int order)
	{
	    // TODO: Implement
	    // Allocate data structures
	}
	/**
	 * Add key to node.
	 * Return 0 on success, -1 on failure
	 */
	int addKeyToNode(uint64_t key)
	{
	    // TODO: Implement
	    return 0;
	}

	/**
	 * Delete key from node
         * Return 0 on success, -1 on failure
	 */
	int deleteKeyFromNode(uint64_t key)
	{
	    // TODO: Implement
	    return 0;
	}

	/**
	 * Check if key is present in this node
	 * Return true if present, false if not
	 */
	bool checkPresenceOfKey(uint64_t key)
	{
            // TODO: Implement
	    return true;
	}

	uint64_t getMaxValue()
	{
	    // TODO: Implement
	    return 0;
	}
};

/**
 * CSB Tree Base Class
 */
class CSBTree
{
    private:
    	CSBTree *root; // Pointer to the root of the CSB tree
    	int d; // order of the CSB tree

    public:
	/**
	 * Init a new CSB tree with the given order
	 */
	CSBTree(int order)
	{
	    d = order;
	}

	/**
	 * Insert a new key index into the CSB tree
	 * Needs to be implemented in derived classes
	 */
	virtual int insert(uint64_t key) = 0;

	/**
	 * Search for a given key
	 */
	int search(uint64_t key)
	{
	    // TODO: Implement
	    return 0;
	}

	/**
	 * Delete a key index from the CSB tree
	 */
	virtual int del(uint64_t key) = 0;

	/**
	 * Print the CSB tree
	 */
	void print()
	{
	    cout<<"Printing!"<<endl;
	    // TODO: Implement
	}
};
