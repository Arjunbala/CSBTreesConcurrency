#include "csbtree.h"

/**
 * Implementation for a full CSB tree
 */
class FullCSBTree : public CSBTree
{
    private:
        CSBNode* insertInternal(CSBNode *root, uint64_t key, int *median)
	{
	}

    public:
        FullCSBTree(int num) : CSBTree(num)
	{
	    // TODO: Implement
	    root = new CSBNode(num);
	}

	int insert(uint64_t key)
	{
	    // TODO: Implement
	    CSBNode *leftChild; // new left child
	    CSBNode *rightChild; // new right child
	    int median;

	    rightChild = insertInternal(root, key, &median);

	    if(rightChild)
	    {
                // need to split the root
		leftChild = new CSBNode(d);
	    }
	    return 0;
	}

	int del(uint64_t key)
        {
            // TODO: Implement
            return 0;
        }
};
