//-----------------------------------------------------------------------------
//  Author      : Sheptunov Dmitry
//  Description : Prototype of deterministic finite-state machine.
//                Set of states is described in state.h
//                Alphabed - in event.h
//                Transition function is represented as a table (state x event)
//-----------------------------------------------------------------------------

#ifndef __DFSM__
#define __DFSM__

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

    // default constructor
    DFSM();

    // method applies transition according to table of transitions
    void do_transition();
};

#endif