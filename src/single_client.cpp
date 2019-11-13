#include"fullcsbtree.h"

int main(int argc, char **argv)
{
    FullCSBTree *root = new FullCSBTree(1);
    root->print();
    root->insert(1);
    root->printInorder();
    root->insert(2);
    root->printInorder();
    root->insert(3);
    root->printInorder();
    return 0;
}
