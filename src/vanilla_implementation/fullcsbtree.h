#include "csbtree.h"

/**
 * Implementation for a full CSB tree
 */
class FullCSBTree : public CSBTree
{
    private:
        CSBNode* insertInternal(CSBNode *root, uint64_t key, uint64_t *median)
	{
            int pos;
	    uint64_t mid;
	    CSBNode *new_node;
	    
	    // First search which position is the right one to start traversing
	    int index = root->getIndex(key);
            if(root->isLeaf)
	    {
	        // Need to insert key here
		printf("FullCSBTree::insertInternal inserting %lu at leaf node\n", key);
		int ret = root->addKeyToNode(key, index);
		// Leaf has no child pointers. So no need to do anything
		if (ret == -1) {
		    // Insertion went wrong
		    printf("FullCSBTree::insertInternal insert failure at leaf for %lu\n", key);
		    exit(1);
		}
	    } else {
                // insert into child node
		printf("FullCSBTree::insertInternal moving to child node at %d for key %lu\n", index, key);
		new_node = insertInternal(root->left_child[index], key, &mid);
		if(new_node)
		{
	            // need to add a new key at this level
		    printf("FullCSBTree::insertInternal adding new key at non-leaf %lu\n", mid);
		    int index_level = root->getIndex(mid);
		    int ret = root->addKeyToNode(mid, index_level);
		    if(ret == -1)
		    {
			printf("FullCSBTree::insertInternal insert failure at non-leaf for %lu\n", key);
		        exit(1);
		    }
		    // Add new child at this level
		    root->left_child[index_level+1] = new_node;
		}
	    }

	    // Now check if we need to split
	    if(root->nKeys > 2*d)
	    {
	        // to split
		printf("FullCSBTree::insertInternal Need to split\n");
		mid = root->nKeys/2;
		*median = root->data[mid];

		// Now we need to make a new node for keys > median
		new_node = new CSBNode(d);
		new_node->nKeys = root->nKeys - mid - 1;
		new_node->isLeaf = root->isLeaf;

		// Now actually move the keys
		for(int i=mid+1;i<root->nKeys;i++)
		{
		    new_node->data[i-(mid+1)] = root->data[i];
		}

		// Now move the child pointers
		if(!root->isLeaf)
		{
	            for(int i=mid+1;i<=root->nKeys;i++)
                    {
                        new_node->left_child[i-(mid+1)] = root->left_child[i];
                    }
		}
		root->nKeys = mid+1;
		return new_node;
	    }
	    return NULL;
	}

    public:
        FullCSBTree(int num) : CSBTree(num)
	{
	    this->root = new CSBNode(num);
	}

	int insert(uint64_t key)
	{
	    CSBNode *leftChild; // new left child
	    CSBNode *rightChild; // new right child
	    uint64_t median;

	    rightChild = insertInternal(root, key, &median);

	    if(rightChild != NULL)
	    {
                // need to split the root
		printf("FullCSBTree::insert root split for key %lu\n", key);
		leftChild = new CSBNode(d);
                int i;
                for(i=0;i<root->nKeys;i++)
                {
                    leftChild->data[i] = root->data[i];
                    leftChild->left_child[i] = root->left_child[i];
                }
                leftChild->left_child[i] = root->left_child[i];
                leftChild->nKeys = root->nKeys;
		this->root->nKeys = 1;
		this->root->isLeaf = false;
		this->root->data[0] = median;
		this->root->left_child[0] = leftChild;
		this->root->left_child[1] = rightChild;
	    }
	    return 0;
	}

	int del(uint64_t key)
        {
            // TODO: Implement
            return 0;
        }
};
