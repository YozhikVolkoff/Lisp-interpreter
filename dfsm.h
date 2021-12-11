#ifndef __DFSM__
#define __DFSM__

#define BUF_SIZE 512

#include <map>
#include "event.h"
#include "state.h"

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

#endif