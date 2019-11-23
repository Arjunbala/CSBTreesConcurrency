#include <stdio.h>
#include <iostream>
#include <stdint.h>
#include <cstdio>
#include <cstring>
#include <stdlib.h>

using namespace std;

/**
 * Representation of a Full CSB Tree node
 */
class CSBNode
{
    public:
		int order;
		uint64_t *data; // Assume 64-bit integer primary key index
		bool isLeaf; // Indicates if this node is a leaf node
		CSBNode *p_child; // Pointer to leftmost child
		int nKeys; // Number of keys currently held by this node
#ifdef LOCKING
                uint64_t high_key; // Highest key value with descendents of this node
                pthread_mutex_t node_lock; // lock associated with this node
#endif

		CSBNode()
		{
	            this->order = 0;
		    this->isLeaf = true;
		    this->data = NULL;
		    this->nKeys = 0;
		    this->p_child = NULL;
#ifdef LOCKING
                    this->high_key = 0;
                    if (pthread_mutex_init(&node_lock, NULL) != 0) { 
                        cout<<"Could not init node lock"<<endl;
                        exit(1);
                    } 
#endif
		}

		/**
		 * Init a new CSB node given the order of the CSB tree
		 */
		CSBNode(int order, bool isLeaf)
		{
		    // Allocate data structures
		    this->order = order;
		    this->isLeaf = isLeaf;
		    // We can have between order and 2*order (incl) keys
		    // Hence allocate an array to store this number of keys
		    this->data = new uint64_t[2*order+3];
		    this->nKeys = 0;
		    this->p_child = NULL;
#ifdef LOCKING
                    this->high_key = 0;
                    if (pthread_mutex_init(&node_lock, NULL) != 0) {
                        cout<<"Could not init node lock"<<endl;
                        exit(1);
                    }
#endif
		}

#ifdef LOCKING
                /**
                 * Acquire an exclusive lock over this node
                 */
                void acquireLock()
                {
                    pthread_mutex_lock(&node_lock);
                }

                /**
                 * Release the exclusive lock over this node
                 */
                void releaseLock()
                {
                    pthread_mutex_unlock(&node_lock);
                }
#endif

		void createData(int order, bool isLeaf)
		{
		    // Allocate data structures
		    this->order = order;
		    this->isLeaf = isLeaf;
		    // We can have between order and 2*order (incl) keys
		    // Hence allocate an array to store this number of keys
		    this->data = new uint64_t[2*order+1];
		    this->nKeys = 0;
		    this->p_child = NULL;
#ifdef LOCKING
                    this->high_key = 0;
#endif
		}

		/**
		 * Copy data from given node
		 */
		void copyFromNode(CSBNode *node)
		{
			this->nKeys = node->nKeys;
			this->p_child = node->p_child;

			for(int i=0;i<this->nKeys;i++)
			{
				this->data[i] = node->data[i];
			}
#ifdef LOCKING
                        this->high_key = node->high_key;
#endif
		}

		/**
		 * Add key to node.
		 * Return index on success, -1 on failure
		 */
		int addKeyToNode(uint64_t key, int index)
		{
			// Insert only if max keys not reached
		    if(this->nKeys+1 <= 2*order+1) {
		        // Find right position to insert at
				if(this->nKeys == 0)
				{
			            // First time inserting key
				    this->data[0] = key;
				    this->nKeys++;
				    // Leave it upto higher level logic to adjust child pointers
				    // It will be NULL by default at this stage
				    return 0;
				}
				else
				{
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
				    	// TODO: Check
				    	// for(int i=this->nKeys-1;i>index;--i)
				    	for(int i=this->nKeys;i>index;--i)
				    	{
				    		(this->p_child+i)->copyFromNode(this->p_child+i-1);
				    	}
						// Fix the two child pointers unknown for now. Higher level will correct it
						(this->p_child+(index+1))->copyFromNode(this->p_child+index);
				    }
				    return index;
				}
		    }
		    // Will handle at higher level since this is done using CoW
		    return -1;
		}

#ifdef LOCKING
                // Return 1 if update to high key, 0 if no update
                int setHighKeyIfNecessary(uint64_t key)
                {
                    if (key > this->high_key) {
                        this->high_key = key;
                         return 1;
                    }
                    return 0;
                }

                // Return high key at this node
                uint64_t getHighKey() {
                    return this->high_key;
                }
#endif

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
		 * Return first index greater than key
		 */
		int getIndex(uint64_t key)
		{
	            // TODO: Use binary search since keys will be sorted
		    int index = 0;
		    while(index < this->nKeys && this->data[index]<key)
		    {
				index++;
		    }
		    return index;
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

                int search(uint64_t key)
                {
                    if(this->isLeaf) {
                        int index = this->getIndex(key);
                        if(data[index] == key) {
                            return 1;
                        } else {
                            return 0;
                        }
                    } else {
                        int index = this->getIndex(key);
                        return (p_child+index)->search(key);
                    }
                }

        void printInorder()
        {
            int i;
            for(i=0;i<this->nKeys;i++)
            {
                if(!this->isLeaf)
                {
                	cout<<"down";
                    (this->p_child+i)->printInorder();
                }
                printf("%lu ", this->data[i]);
            }
            if(i>0 && !this->isLeaf)
            {
            	cout<<"down";
                (this->p_child+i)->printInorder();
            }
            printf("\n");
        }

        void printData()
        {
            for(int i=0;i<this->nKeys;i++)
            {
                printf("%lu ", this->data[i]);
            }
            printf("\n");
        }
};

/**
 * CSB Tree Base Class
 */
class CSBTree
{
    public:
    	CSBNode *root; // Pointer to the root of the CSB tree
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
	    return root->search(key);
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

        /**
         * Print inorder traversal
         */
        void printInorder()
        {
            root->printInorder();
        }
};