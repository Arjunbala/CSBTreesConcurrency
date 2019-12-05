#include "fullcsbnode.h"

/**
 * Implementation for a full CSB tree
 */
class FullCSBTree : public CSBTree
{
    private:
        bool insertInternal(CSBNode *root, uint64_t key, bool &is_failed)
		{
		    
		    // First search which position is the right one to start traversing
		    int index = root->getIndex(key);
		    if(root->isLeaf)
		    {
		        // Need to insert key here
				printf("FullCSBTree::insertInternal inserting %lu at leaf node\n", key);
#ifdef LOCKING
                                root->acquireLock();
#endif
				int ret = root->addKeyToNode(key, index);	
#ifdef LOCKING
                                root->releaseLock();
#endif
				// Leaf has no child pointers. So no need to do anything
				if (ret == -1)
				{
				    // Insertion went wrong
				    printf("FullCSBTree::insertInternal insert failure at leaf for %lu\n", key);
				    is_failed = true;
				    return false;
				}
				root->setHighKeyIfNecessary(key);
		    }
		    else
		    {
				printf("FullCSBTree::insertInternal moving to child node at %d for key %lu\n", index, key);
                (root->p_child+index)->setHighKeyIfNecessary(key);
				bool child_split = insertInternal(root->p_child+index, key, is_failed);

				// Some failure at or below child level
				if(is_failed)
				{
                   	return false;
				}
                                
				if(child_split)
				{
					CSBNode *old_child = root->p_child+index;
#ifdef LOCKING
                                        // Acquire lock over current node so that we can modify pointers
                                        root->acquireLock();
                                        // Acquire lock over fully filled child so that we can split it
                                        old_child->acquireLock();
#endif
                                        cout<<"Printing root and root+1 address: ";
                                        cout<<root<<", "<<root+1<<"\n";
                                        cout<<"Size of node: "<<sizeof(root)<<"\n";
                                        cout<<"Printing root and root+sizeof(root) address: ";
                                        cout<<root<<", "<<root+sizeof(root)<<"\n";

                                        // We may have a case here where the current node has split.
                                        // Hence, we need to keep moving right to find the right parent for this child
                                        while(key > root->getHighKey() && (root+1) != NULL) {
#ifdef LOCKING
                                            // acquire lock over next sibling
                                            (root+1)->acquireLock();
                                            // release lock over current sibling
                                            root->releaseLock();
#endif
                                            root = root+1;
                                        }

                    // Case when parent got moved to new node group
                    if(key > root->getHighKey())
                    {
                    	is_failed = true;
                    	return false;
                    }
					bool is_child_leaf = old_child->isLeaf;
					int old_nKeys = old_child->nKeys;
					int mid_ind = old_nKeys/2;
					uint64_t mid = old_child->data[old_nKeys/2];

					// need to add a new key at this level
				    printf("FullCSBTree::insertInternal adding new key at non-leaf %lu\n", mid);

				    // Also shifts children by 1 after index position
				    int ret = root->addKeyToNode(mid, index);
				    if(ret == -1)
				    {
						printf("FullCSBTree::insertInternal insert failure at non-leaf for %lu\n", key);
				    	is_failed = true;
                    	return false;
				    }

				    // copy 2nd half of data into node next to child
				    for(int i=mid_ind+1;i<old_nKeys;i++)
				    {
				    	(old_child+1)->data[i-(mid_ind+1)] = old_child->data[i];
				    }

				    // Set number keys in old and new child
				    old_child->nKeys = mid_ind+1;
				    (old_child+1)->nKeys = old_nKeys - (mid_ind + 1);


				    // if child is not a leaf create new node group and split in half
				    if(!is_child_leaf)
				    {
				    	bool is_gchild_leaf = old_child->p_child->isLeaf;
				    	CSBNode *new_ngrp = createNodeGroup(d, is_gchild_leaf);
				    	(old_child+1)->p_child = new_ngrp;

				    	for(int i=mid_ind+1;i<=old_nKeys;i++)
				    	{
				    		(new_ngrp+i-(mid_ind+1))->copyFromNode(old_child->p_child+i);
				    	}

				    	// Median moves up if child is not leaf
				    	old_child->nKeys = mid_ind;

				    	//Add correct old child high key
				    	if ((old_child->p_child+mid_ind)->nKeys>0)
				    	{
				    		//Rightmost grandchild node has data so use its high key
				    		old_child->high_key = (old_child->p_child+mid_ind)->high_key;
				    	}
				    	else
				    	{
				    		//Rightmost grandchild node has no data so use high key of prev
				    		old_child->high_key = (old_child->p_child+mid_ind-1)->high_key;
				    	}
				    }
#ifdef LOCKING
                                    // Release locks
                                    old_child->releaseLock();
                                    root->releaseLock();
#endif
				}
		    }

		    // Now check if we need to split
		    if(root->nKeys > 2*d)
		    {
		        // Split should happen, inform parent
				printf("FullCSBTree::insertInternal Need to split\n");
				return true;
		    }
		    return false;
		}

		CSBNode* createNodeGroup(int order, bool isLeaf)
		{
			// TODO: Find better way to do this
			CSBNode* ret = new CSBNode[2*order+2];
		    for(int i=0;i<2*order+2;i++)
		    {
		    	(ret+i)->createData(order, isLeaf);
		    }
			return ret;
		}

    public:
        FullCSBTree(int order) : CSBTree(order)
		{
			// TODO: Find better way to do this
		    this->root = new CSBNode[2*order+3];
		    for(int i=0;i<2*order+2;i++)
		    {
		    	(this->root+i)->createData(order,true);
		    }
		}

		int insert(uint64_t key)
		{
			bool is_failed = false;
		    bool is_root_split = insertInternal(root, key, is_failed);

		    // insertion failed
		    if(is_failed){
		    	return -1;
		    }

		    if(is_root_split)
			{
#ifdef LOCKING
                                // Acquire lock over root
                                root->acquireLock();
#endif
				// Create new node group for new root
				CSBNode *new_root = createNodeGroup(d, false);
				int old_nKeys = root->nKeys;
				int mid_ind = old_nKeys/2;
				uint64_t median = root->data[old_nKeys/2];

				// Add median to root
				new_root->addKeyToNode(median, 0);
				// set old root as child of new root
				new_root->p_child=root;

				// copy 2nd half of data into node next to root
				for(int i=mid_ind+1;i<old_nKeys;i++)
			    {
			    	(root+1)->data[i-(mid_ind+1)] = root->data[i];
			    }

			    // Set number of keys in old and new child
			    root->nKeys = mid_ind+1;
			    (root+1)->nKeys = old_nKeys - (mid_ind + 1);

			    // Move 2nd half of grandchildren of new root into new nodegroup
			    if(!root->isLeaf)
				{
					bool is_child_leaf = root->p_child->isLeaf;
			    	CSBNode *new_ngrp = createNodeGroup(d, is_child_leaf);
			    	(root+1)->p_child = new_ngrp;

			    	for(int i=mid_ind+1;i<=old_nKeys;i++)
			    	{
			    		(new_ngrp+i-(mid_ind+1))->copyFromNode((root)->p_child+i);
			    	}

			    	// Median moves up if child is not leaf
			    	root->nKeys = mid_ind;

			    	//Add correct old child high key
			    	if ((root->p_child+mid_ind)->nKeys>0)
			    	{
			    		//Rightmost grandchild node has data so use its high key
			    		root->high_key = (root->p_child+mid_ind)->high_key;
			    	}
			    	else
			    	{
			    		//Rightmost grandchild node has no data so use high key of prev
			    		root->high_key = (root->p_child+mid_ind-1)->high_key;
			    	}
				}

			    // Assign new root as root
			    root=new_root;
#ifdef LOCKING
                            root->releaseLock();
#endif
			}

		    return 0;
		}

		int del(uint64_t key)
		{
            // TODO: Implement
            return 0;
        }
};
