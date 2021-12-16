//-----------------------------------------------------------------------------
//  Author      : Sheptunov Dmitry
//  Description : Declarations of possible actions
//-----------------------------------------------------------------------------

#ifndef __ACTION__
#define __ACTION__

#include "dfsm.h"
#include "lexer.h"
#include "parser.h"

class DFSM;

// base class for all actions
class Action
{
public:
    Action() {}

    virtual ~Action() {}

    // applies action
    virtual void apply() {}
};

// does nothing
class NoAction : public Action
{
public:
    NoAction() : Action() {}

    virtual void apply() {}
};

// base class for the lexical analyzer actions
class LexAction : public Action
{
public:
    Lexer* m_lexer;

    LexAction(Lexer* lexer) : m_lexer(lexer) {}

    virtual ~LexAction() {}

    virtual void apply() {}
};

// go to the next symbol of the input
class GoNext : public LexAction
{
public:
    GoNext(Lexer* lexer) : LexAction(lexer) {}

    virtual void apply();
};

// add current symbol to the buffer and go to the next symbol
class Push_GoNext : public GoNext
{
public:
    Push_GoNext(Lexer* lexer) : GoNext(lexer) {}

    virtual void apply();
};

// extract from the buffer last symbol and go to the next symbol
class Pop_GoNext : public GoNext
{
public:
    Pop_GoNext(Lexer* lexer) : GoNext(lexer) {}

    virtual void apply();
};

// get token from the buffer, put the token to the output,
// clear the buffer
class GetTok : public LexAction
{
public:
    NameType m_name_type;

    GetTok(Lexer* lexer, NameType name_type = NameType::UNDEF) 
        : LexAction(lexer),
        m_name_type(name_type) {}

    virtual void apply();
};

// action for one-symbol token processing;
// put current symbol to the buffer, get token from the buffer,
// go to the next symbol
class Push_GetTok_GoNext : public GetTok
{
public:
    Push_GetTok_GoNext(Lexer* lexer, NameType name_type = NameType::UNDEF) 
        : GetTok(lexer, name_type) {}

    virtual void apply();
};

// action for '(' processing; similar to the Push_GetTok_GoNext
class GetLeftPar : public Push_GetTok_GoNext
{
public:
    GetLeftPar(Lexer* lexer, NameType name_type = NameType::UNDEF)
        : Push_GetTok_GoNext(lexer, name_type) {}

    virtual void apply();
};

// action for ')' processing; similar to the Push_GetTok_GoNext
class GetRightPar : public Push_GetTok_GoNext
{
public:
    GetRightPar(Lexer* lexer, NameType name_type = NameType::UNDEF)
        : Push_GetTok_GoNext(lexer, name_type) {}

    virtual void apply();
};

// base class for parser actions
class ParserAction : public Action
{
public:
    Parser* m_parser;

    ParserAction(Parser* parser) : m_parser(parser) {}

    virtual void apply() {}
};

// add a node to the parsing tree, go down to this node;
// this corresponds to adding an operator argument 
class AddArg : public ParserAction
{
public:
    AddArg(Parser* parser) : ParserAction(parser) {}

    virtual void apply();
};

// add a node to the parsing tree, go down to this node;
// this corresponds to adding an operator 
class AddOp : public ParserAction
{
public:
    AddOp(Parser* parser) : ParserAction(parser) {}

    virtual void apply();
};

// skip current token and apply AddOp
class Skip_AddOp : public AddOp
{
public:
    Skip_AddOp(Parser* parser) : AddOp(parser) {}

    virtual void apply();
};

// go to the parent of the current node
class GoToParent : public ParserAction
{
public:
    GoToParent(Parser* parser) : ParserAction(parser) {}

    virtual void apply();
};

// report an error "undefined token", abort execution
class Err_UndefName : public ParserAction
{
public:
    Err_UndefName(Parser* parser) : ParserAction(parser) {}

    virtual void apply();
};

#endif