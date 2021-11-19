#ifndef __DFSM__
#define __DFSM__

#include<vector>
#include <map>
#include "token.h"
#include "transition.h"
#include "event.h"

class Transition;

class DFSM
{
public:
    State m_curr_state;
    Event m_curr_event;
    std::map<State, std::map<Event, Transition> > m_transit_table;
    std::string m_input;
    std::string::iterator m_curr_char;
    std::string m_buffer;
    std::vector<Token> m_output;


    DFSM() : m_curr_state(State::INIT), m_curr_event(Event::NO_EVENT) {}
    
    void set_input(std::string input);

    void do_transition();

    virtual void set_event() {}
};

class Preproc : public DFSM
{
public:
    Preproc();

    virtual void set_event();
};

class Parser : public DFSM
{
public:
    Parser();

    virtual void set_event();
};

#endif