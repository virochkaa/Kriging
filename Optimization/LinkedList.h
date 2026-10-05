#pragma once
#include "Populations.h"
#include "DataBank.h"

struct list
{
    int index = 0;
    struct list *parent = nullptr;
    struct list *child = nullptr;
};
/* Insert an element X into the list at location specified by NODE */
void insertNode (list* node, int _index);

/* Delete the node NODE from the list */
list* deleteNode(list*& node);

/*Инициализация list*/
void initList(list*& lst);

void DeleteList(list*& node);