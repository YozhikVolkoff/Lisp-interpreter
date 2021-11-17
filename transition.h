#ifndef __TRANSITION__
#define __TRANSITION__

#include "action.h"

class Transition
{
public:
    Action m_action;
    State m_target_state;

    Transition() : m_target_state(State::LAST_ONE) {}
    Transition(Action action, State t_state) : m_action(action), m_target_state(t_state) {}
};

#endif