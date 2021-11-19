#ifndef __ACTION__
#define __ACTION__

class DFSM;

class Action
{
public:
    DFSM* m_dfsm;

    Action(DFSM* dfsm = nullptr) : m_dfsm(dfsm) {}

    virtual void apply() {}
};

class NoAction : public Action
{
public:
    virtual void apply() {}
};

#endif