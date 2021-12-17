//-----------------------------------------------------------------------------
//  Author      : Sheptunov Dmitry
//  Description : Lisp program parser
//                Child class for finite-state machine (dfsm.h)
//                m_root - root of the parsing tree
//                m_in - input - token flow
//-----------------------------------------------------------------------------

#ifndef __PARSER__
#define __PARSER__

#include "dfsm.h"
#include "parsing_tree.h"

class Parser : public DFSM
{
private:
    Node* m_root;
    Node* m_curr_node;
    std::vector<Name>::iterator m_curr_name;
    std::vector<Name> m_in;

public:
    // default constructor
    Parser();

    // load token flow to the parser
    void set_input(std::vector<Name> input);

    Node* get_output() const;

    // map a token to an event
    Event to_event(const Name& name);

    // allow actions with the parser
    friend class ParserAction;
    friend class AddArg;
    friend class AddOp;
    friend class Skip_AddOp;
    friend class GoToParent;
    friend class Err_UndefName;
};

#endif