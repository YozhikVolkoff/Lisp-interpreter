#include "transition.h"

DFSM::DFSM() 
    : m_curr_state(State::INIT), 
    m_curr_event(Event::NO_EVENT), 
    m_curr_transit(nullptr) {}

void DFSM::do_transition() {
    m_curr_transit = m_transit_table[m_curr_state][m_curr_event];
    m_curr_transit->m_action->apply();
    m_curr_state = m_curr_transit->m_target_state;
}