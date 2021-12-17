//-----------------------------------------------------------------------------
//  Author      : Vasyuk Vladimir
//  Description : Evaluator executes the parsing tree
//-----------------------------------------------------------------------------

#ifndef __EVALUATOR__
#define __EVALUATOR__

#include <list>
#include <iostream>
#include "parsing_tree.h"
#include "parser.h"
#include "table.h"
#include "error.h"

using std::string;
using std::list;

class Evaluator
{
public:
    Node* m_curr_node;

    Evaluator(Node* curr): m_curr_node(curr) {}
    
    Value* eval(Node* сurr);
};

#endif
