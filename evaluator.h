#ifndef __EVALUATOR__
#define __EVALUATOR__

#include <list>
#include "parsing_tree.h"
#include "evaluator.h"

using std::Node;
using std::Name;
using std::NameType;

class Evaluator()
{
public:
    Node* m_curr_node;

    Evaluator(Node* curr): m_curr_node(curr) {}
    
    void eval(Node* сurr);
}

#endif
