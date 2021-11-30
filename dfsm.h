#ifndef __DFSM__
#define __DFSM__

#define BUF_SIZE 512

#include<vector>
#include <map>
#include <cassert>
#include "event.h"
#include "state.h"
#include "parsing_tree.h"

class Transition;

class DFSM
{
public:
    State m_curr_state;
    Event m_curr_event;
    Transition* m_curr_transit;
    std::map<State, std::map<Event, Transition*> > m_transit_table;

    DFSM();

    void do_transition();
};

class Lexer : public DFSM
{
public:
    std::map<char, Event> m_event_table;
    std::string m_in;
    std::string::iterator m_curr_char;
    std::string m_buffer;
    int m_parenth_count;
    std::vector<Name> m_out;

    Lexer();

    void set_input(std::string input);
};

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