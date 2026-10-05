#include "LinkedList.h"

void insertNode(list *node, int _index)
{
    list *temp = nullptr;
    Memory::AllocateObj(temp);
    temp->index = _index;
    temp->child = node->child;
    temp->parent = node;
    if (node->child)
        node->child->parent = temp;
    node->child = temp;
}

list *deleteNode(list *&node)
{
    list *myParent = node->parent;
    list *myChild = node->child;

    if (myParent)
    {
        myParent->child = myChild;
        if (myChild)
            myChild->parent = myParent;
    }

    Memory::DeallocateObj(node);
    return myParent;
}

void initList(list *&lst)
{
    Memory::AllocateObj(lst);
    lst->index = -1;
    lst->parent = nullptr;
    lst->child = nullptr;
}

void DeleteList(list *&node)
{
    if (node->child != nullptr)
        DeleteList(node->child);
    Memory::DeallocateObj(node);
}