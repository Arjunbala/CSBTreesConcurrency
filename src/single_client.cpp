#include"fullcsbtree.h"

int main(int argc, char **argv)
{
    FullCSBTree *root = new FullCSBTree(1);
    root->print();
    root->insert(1);
    root->insert(2);
    root->insert(3);
    return 0;
}
