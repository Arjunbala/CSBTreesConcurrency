#include <stdio.h>
#include <iostream>
#include <stdint.h>

using namespace std;

/**
 * Representation of a CSB Tree node
 */
class CSBNode
{
    public:
	int order;
        uint64_t *data; // Assume 64-bit integer primary key index
        bool isLeaf; // Indicates if this node is a leaf node
        CSBNode **left_child; // Pointer to leftmost child
        int nKeys; // Number of keys currently held by this node

	/**
	 * Init a new CSB node given the order of the CSB tree
	 */
	CSBNode(int order)
	{
	    // Allocate data structures
	    this->order = order;
	    // We can have between order and 2*order (incl) keys
	    // Hence allocate an array to store this number of keys
	    this->data = new uint64_t[2*order];
	    this->isLeaf = true;
	    this->nKeys = 0;
	    // Preallocate an array which hold pointers to child, but store reference only to leftmost child
	    // if there are 2*order keys, we can have upto 2*order+1 pointers to children
	    this->left_child = new CSBNode*[2*order+1];
	    // Initially they point to nothing. Explicitly de-reference them to avoid leaving them dangling
	    for(int i=0;i<2*order+1;i++)
	    {
               this->left_child[i] = NULL; 
	    }
	}
	/**
	 * Add key to node.
	 * Return index on success, -1 on failure
	 */
	int addKeyToNode(uint64_t key)
	{
	    if(this->nKeys < 2*order) {
	        // Find right position to insert at
		if(this->nKeys == 0)
		{
	            // First time inserting key
		    this->data[0] = key;
		    // Leave it upto higher level logic to adjust child pointers
		    // It will be NULL by default at this stage
		    return 0;
		}
		else
		{
		    // Find the right index for new key
		    int index = 0;
		    // TODO: Can use binary-search based techniques since keys are stored?
		    while(index < this->nKeys && this->data[index] < key)
		    {
		        index++;
		    }
		    // Need to move everything right and put this key at 'index'
		    for(int i=this->nKeys-1;i>=index;--i)
		    {
		        this->data[i+1] = this->data[i];
		    }
		    // Now place the key
		    this->data[index] = key;
		    this->nKeys++;
		    // Now need to do the same for child pointers if a non-leaf node
		    if(!this->isLeaf)
		    {
                        for(int i=this->nKeys-1;i>index;--i)
			{
		            this->left_child[i+1] = this->left_child[i];
			}
			// Fix the two child pointers unknown for now. Higher level will correct it
			this->left_child[index+1] = this->left_child[index];
		    }
		    return index;
		}
	    }
	    // Else, we require the node to split
	    // Will handle at higher level since this is done using CoW
	    return -1;
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
            // TODO: Use binary search since keys will be sorted
	    for(int i=0;i<this->nKeys;i++)
	    {
	        if(this->data[i] == key) 
		{
		    return true;
		}
	    }
	    return false;
	}

	/**
	 * Get maximum key within this node
	 */
	uint64_t getMaxValue()
	{
	    if(this->nKeys > 0)
	    {
	        return this->data[nKeys-1];
	    }
	    return 0;
	}
};

/**
 * CSB Tree Base Class
 */
class CSBTree
{
    protected:
    	CSBNode *root; // Pointer to the root of the CSB tree
    	int d; // order of the CSB tree

    public:
	/**
	 * Init a new CSB tree with the given order
	 */
	CSBTree(int order)
	{
	    d = order;
	    root = NULL;
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
