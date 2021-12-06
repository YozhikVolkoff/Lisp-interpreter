#ifndef __EVALUATOR__
#define __EVALUATOR__

class Evaluator()
{
public:
    Node* m_root;
    Node* m_curr_node;
    std::vector<Name>::iterator m_curr_name;
    std::vector<Name> m_in;

    Evaluator();

    void eval(Node* root);
}

#endif
