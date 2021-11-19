#ifndef __TRANSITION__
#define __TRANSITION__

#include "action.h"
#include "state.h"

class Transition
{
public:
    Action m_action;
    State m_target_state;

    Transition(Action action = Action(), State t_state = State::TERM) 
        : m_action(action), 
        m_target_state(t_state) {}
};

#endif