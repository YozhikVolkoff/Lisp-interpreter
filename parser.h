#ifndef __PARSER__
#define __PARSER__

#include "dfsm.h"
#include "parsing_tree.h"

class Parser : public DFSM
{
public:
    Node* m_root;
    Node* m_curr_node;
    std::vector<Name>::iterator m_curr_name;
    std::vector<Name> m_in;

    Parser();

    void set_input(std::vector<Name> input);

    Event to_event(const Name& name);
};

#endif