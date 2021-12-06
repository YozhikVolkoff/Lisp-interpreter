#ifndef __EVALUATOR__
#define __EVALUATOR__

using std::list;

class Evaluator()
{
public:
    Node* m_curr_node;

    Evaluator(Node* curr): m_curr_node(curr) {}
    
    void eval(Node* сurr);
}

#endif
