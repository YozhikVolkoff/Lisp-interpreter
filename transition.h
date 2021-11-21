#ifndef __TRANSITION__
#define __TRANSITION__

#include "dfsm.h"

class Transition
{
public:
    DFSM* m_dfsm;
    State m_target_state;

    Transition(DFSM* dfsm = nullptr, State t_state = State::TERM) 
        : m_dfsm(dfsm), 
        m_target_state(t_state) {}

    virtual void apply() {
        m_dfsm->m_curr_state = m_target_state;
    }
};

class Transit_GoNext : public Transition
{
public:
    Transit_GoNext(DFSM* dfsm = nullptr, State t_state = State::TERM) : Transition(dfsm, t_state) {}

    virtual void apply() {
        m_dfsm->m_curr_char++;
        m_dfsm->m_curr_event = m_dfsm->m_event_table[*m_dfsm->m_curr_char];
        Transition::apply();
    }
};

class Transit_Wr_GoNext : public Transition
{
public:
    Transit_Wr_GoNext(DFSM* dfsm = nullptr, State t_state = State::TERM) : Transition(dfsm, t_state) {}

    virtual void apply() {
        m_dfsm->m_buffer.append(1, *m_dfsm->m_curr_char);
        m_dfsm->m_curr_char++;
        m_dfsm->m_curr_event = m_dfsm->m_event_table[*m_dfsm->m_curr_char];
        Transition::apply();
    }
};

class Transit_GetTok : public Transition
{
public:
    Transit_GetTok(DFSM* dfsm = nullptr, State t_state = State::TERM) : Transition(dfsm, t_state) {}

    virtual void apply() {
        m_dfsm->m_out.push_back(Token(m_dfsm->m_buffer));
        m_dfsm->m_buffer.clear();
        Transition::apply();
    }
};

class Transit_Wr_GetTok_GoNext : public Transition
{
public:
    Transit_Wr_GetTok_GoNext(DFSM* dfsm = nullptr, State t_state = State::TERM) : Transition(dfsm, t_state) {}

    virtual void apply() {
        m_dfsm->m_buffer.append(1, *m_dfsm->m_curr_char);
        m_dfsm->m_out.push_back(Token(m_dfsm->m_buffer));
        m_dfsm->m_buffer.clear();
        m_dfsm->m_curr_char++;
        m_dfsm->m_curr_event = m_dfsm->m_event_table[*m_dfsm->m_curr_char];
        Transition::apply();
    }
};

#endif