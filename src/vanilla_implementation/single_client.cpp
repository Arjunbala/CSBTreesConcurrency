#include"fullcsbtree2.h"

int main(int argc, char **argv)
{
	FullCSBTree *root = new FullCSBTree(1);
    root->insert(10);
    root->printInorder();
    root->insert(5);
    root->printInorder();
    root->insert(2);
    root->printInorder();
    root->insert(1);
    root->printInorder();
    root->insert(30);
    root->printInorder();
    root->insert(20);
    root->printInorder();
    root->insert(25);
    root->printInorder();
    cout<<root->search(25)<<endl;
    cout<<root->search(10)<<endl;
    cout<<root->search(40)<<endl;
    cout<<root->search(1)<<endl;
    // cout<<root->root->data[0]<<"\n";
    // cout<<root->root->p_child->nKeys<<": nKeys\n";
    // cout<<root->root->p_child->data[0]<<"\n";
    // cout<<root->root->p_child->p_child->nKeys<<": nKeys\n";
    // cout<<root->root->p_child->p_child->data[0]<<"\n";
    // cout<<root->root->p_child->p_child->data[1]<<"\n";
    // cout<<(root->root->p_child->p_child+1)->nKeys<<": nKeys\n";
    // cout<<(root->root->p_child->p_child+1)->data[0]<<"\n";
    // cout<<(root->root->p_child->p_child+1)->data[1]<<"\n";
    // cout<<(root->root->p_child+1)->nKeys<<": nKeys\n";
    // cout<<(root->root->p_child+1)->data[0]<<"\n";
    // cout<<(root->root->p_child+1)->p_child->nKeys<<": nKeys\n";
    // cout<<(root->root->p_child+1)->p_child->data[0]<<"\n";
    // cout<<((root->root->p_child+1)->p_child+1)->nKeys<<": nKeys\n";
    // cout<<((root->root->p_child+1)->p_child+1)->data[0]<<"\n";
    // cout<<((root->root->p_child+1)->p_child+1)->data[1]<<"\n";
    

    cout<<"\n";
    return 0;
}
