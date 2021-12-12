#ifndef __PARSING_TREE__
#define __PARSING_TREE__

#include <list>
#include "name.h"

class Node
{
public:
    Name* m_name;
    Node* m_parent;
    std::list<Node*> m_chld;

    Node(Name* name, Node* parent = nullptr) : m_name(name), m_parent(parent) {}
};

#endif