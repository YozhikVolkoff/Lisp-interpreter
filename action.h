/*#ifndef __ACTION__
#define __ACTION__

#include "dfsm.h"

class DFSM;

class Action
{
public:
    DFSM* m_dfsm;

    Action(DFSM* dfsm = nullptr) : m_dfsm(dfsm) {}

    virtual void apply() {}
};

class GoNext : public Action
{
public:
    GoNext(DFSM* dfsm) : Action(dfsm) {}

    virtual void apply() {
        m_dfsm->m_curr_char ++;
        m_dfsm->m_curr_event = m_dfsm->m_event_table[*m_dfsm->m_curr_char];
    }
};

class Wr_GoNext : public Action
{
public:
    Wr_GoNext(DFSM* dfsm) : Action(dfsm) {}

    virtual void apply() {
        m_dfsm->m_buffer.append(1, *m_dfsm->m_curr_char);
        m_dfsm->m_curr_char ++;
        m_dfsm->m_curr_event = m_dfsm->m_event_table[*m_dfsm->m_curr_char];
    }
};

class GetTok : public Action
{
public:
    GetTok(DFSM* dfsm) : Action(dfsm) {}

    virtual void apply() {
        m_dfsm->m_out.push_back(Token(m_dfsm->m_buffer));
        m_dfsm->m_buffer.clear();
    }
};

class Wr_GetTok_GoNext : public Action
{
public:
    Wr_GetTok_GoNext(DFSM* dfsm) : Action(dfsm) {}

    virtual void apply() {
        m_dfsm->m_buffer.append(1, *m_dfsm->m_curr_char);
        m_dfsm->m_out.push_back(Token(m_dfsm->m_buffer));
        m_dfsm->m_buffer.clear();
        m_dfsm->m_curr_char++;
        m_dfsm->m_curr_event = m_dfsm->m_event_table[*m_dfsm->m_curr_char];
    }
};

#endif
*/