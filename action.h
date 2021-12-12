#ifndef __ACTION__
#define __ACTION__

#include "dfsm.h"
#include "lexer.h"
#include "parser.h"

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

// lex actions
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

    GetTok(Lexer* lexer, NameType name_type = NameType::UNDEF) 
        : LexAction(lexer),
        m_name_type(name_type) {}

    virtual void apply();
};

class Push_GetTok_GoNext : public GetTok
{
public:
    Push_GetTok_GoNext(Lexer* lexer, NameType name_type = NameType::UNDEF) 
        : GetTok(lexer, name_type) {}

    virtual void apply();
};

class GetLeftPar : public Push_GetTok_GoNext
{
public:
    GetLeftPar(Lexer* lexer, NameType name_type = NameType::UNDEF)
        : Push_GetTok_GoNext(lexer, name_type) {}

    virtual void apply();
};

class GetRightPar : public Push_GetTok_GoNext
{
public:
    GetRightPar(Lexer* lexer, NameType name_type = NameType::UNDEF)
        : Push_GetTok_GoNext(lexer, name_type) {}

    virtual void apply();
};

// parser actions
class PrsrAction : public Action
{
public:
    Parser* m_parser;

    PrsrAction(Parser* parser) : m_parser(parser) {}

    virtual void apply() {}
};

class AddArg : public PrsrAction
{
public:
    AddArg(Parser* parser) : PrsrAction(parser) {}

    virtual void apply();
};

class AddOp : public PrsrAction
{
public:
    AddOp(Parser* parser) : PrsrAction(parser) {}

    virtual void apply();
};

class Skip_AddOp : public AddOp
{
public:
    Skip_AddOp(Parser* parser) : AddOp(parser) {}

    virtual void apply();
};

class GoToParent : public PrsrAction
{
public:
    GoToParent(Parser* parser) : PrsrAction(parser) {}

    virtual void apply();
};

class Err_UndefName : public PrsrAction
{
public:
    Err_UndefName(Parser* parser) : PrsrAction(parser) {}

    virtual void apply();
};

#endif