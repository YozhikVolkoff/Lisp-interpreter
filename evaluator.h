#ifndef __EVALUATOR__
#define __EVALUATOR__

#include <list>
#include <iostream>
#include "parsing_tree.h"

using std::string;
using std::list;

class Evaluator
{
public:
    Node* m_curr_node;

    Evaluator(Node* curr): m_curr_node(curr) {}
    
    Node* eval(Node* сurr);
};

#endif
