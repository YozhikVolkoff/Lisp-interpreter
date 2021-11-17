#ifndef __ACTION__
#define __ACTION__

class Action
{
public:
    Lexer* m_lexer;

    Action(Lexer* lexer = nullptr) : m_lexer(lexer) {}

    virtual int apply();
};

class FreeBuffer : public Action
{
public:
    FreeBuffer(Lexer* lexer = nullptr) : Action(lexer) {}

    virtual int apply();
};

#endif