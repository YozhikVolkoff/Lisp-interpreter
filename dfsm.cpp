#include "transition.h"

// it`s assumed a new machine to be in initial state
// other fields are initialized by exceptional values
DFSM::DFSM() 
    : m_curr_state(State::INIT), 
    m_curr_event(Event::NO_EVENT), 
    m_curr_transit(nullptr) {}


void DFSM::do_transition() {
    // a transition generally changes state and event,
    // because of that we need to hold the current transition
    m_curr_transit = m_transit_table[m_curr_state][m_curr_event];
    // applying action corresponding to the transition
    m_curr_transit->m_action->apply();
    // changing the current state
    m_curr_state = m_curr_transit->m_target_state;
}