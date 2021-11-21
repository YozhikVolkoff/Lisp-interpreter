#ifndef __DFSM__
#define __DFSM__

#define BUF_SIZE 512

#include<vector>
#include <map>
#include "token.h"
#include "event.h"
#include "state.h"

class Transition;

class DFSM
{
public:
    State m_curr_state;
    Event m_curr_event;
    std::map<State, std::map<Event, Transition*> > m_transit_table;
    std::map<char, Event> m_event_table;
    std::string m_in;
    std::string::iterator m_curr_char;
    std::string m_buffer;
    std::vector<Token> m_out;


    DFSM();
    
    void set_input(std::string input);

    void do_transition();
};

class Preproc : public DFSM
{
public:
    Preproc();
};

class Parser : public DFSM
{
public:
    Parser();
};

#endif