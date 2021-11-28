#ifndef __ACTION__
#define __ACTION__

#include "dfsm.h"

class DFSM;

class Action
{
public:
    Action() {}

    virtual ~Action() {}

    virtual void apply() {}
};

class NoAction : public Action
{
public:
    NoAction() : Action() {}

    virtual void apply() {}
};

class LexAction : public Action
{
public:
    Lexer* m_lexer;

    LexAction(Lexer* lexer) : m_lexer(lexer) {}

    virtual ~LexAction() {}

    virtual void apply() {}
};

class GoNext : public LexAction
{
public:
    GoNext(Lexer* lexer) : LexAction(lexer) {}

    virtual void apply();
};

class Push_GoNext : public LexAction
{
public:
    Push_GoNext(Lexer* lexer) : LexAction(lexer) {}

    virtual void apply();
};

class Pop_GoNext : public LexAction
{
public:
    Pop_GoNext(Lexer* lexer) : LexAction(lexer) {}

    virtual void apply();
};

class GetTok : public LexAction
{
public:
    NameType m_name_type;

    GetTok(Lexer* lexer, NameType name_type = NameType::NOT_DEFINED) 
        : LexAction(lexer),
        m_name_type(name_type) {}

    virtual void apply();
};

class Push_GetTok_GoNext : public GetTok
{
public:
    Push_GetTok_GoNext(Lexer* lexer, NameType name_type = NameType::NOT_DEFINED) 
        : GetTok(lexer, name_type) {}

    virtual void apply();
};

class GetLeftPar : public Push_GetTok_GoNext
{
public:
    GetLeftPar(Lexer* lexer, NameType name_type = NameType::NOT_DEFINED)
        : Push_GetTok_GoNext(lexer, name_type) {}

    virtual void apply();
};

class GetRightPar : public Push_GetTok_GoNext
{
public:
    GetRightPar(Lexer* lexer, NameType name_type = NameType::NOT_DEFINED)
        : Push_GetTok_GoNext(lexer, name_type) {}

    virtual void apply();
};

#endif