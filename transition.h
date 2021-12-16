//-----------------------------------------------------------------------------
//  Author      : Sheptunov Dmitry
//  Description : Transition of finite-state machine
//                Contains terminate state of the transition and action
//-----------------------------------------------------------------------------


#ifndef __TRANSITION__
#define __TRANSITION__

#include "action.h"

class Transition
{
public:
    Action* m_action;
    State m_target_state;

    // default constructor
    Transition(Action* action = nullptr, State t_state = State::TERM) 
        : m_action(action), 
        m_target_state(t_state) {}
};

#endif