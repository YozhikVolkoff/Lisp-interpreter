//-----------------------------------------------------------------------------
//  Author      : Sheptunov Dmitry
//  Description : Lexical analyzer
//                Child class for finite-state machine (dfsm.h)
//                m_event_table is a map: <symbol> -> <element of the alphabed>
//                m_in - text of the lisp program
//                m_curr_char - current symbol
//                m_buffer - memory of the automaton
//                m_parenth_counter - parenthesis counter (must be 0 after 
//                   stopping the analyzer in the case of the correct lisp 
//                   program)
//                m_out - the result of the work - token flow
//-----------------------------------------------------------------------------

#ifndef __LEXER__
#define __LEXER__

#include "dfsm.h"
#include "name.h"

class Lexer : public DFSM
{
private:
    std::map<char, Event> m_event_table;
    std::string m_in;
    std::string::iterator m_curr_char;
    std::string m_buffer;
    int m_parenth_count;
    std::vector<Name> m_out;

public:
    // default constructor
    Lexer();

    // load lisp program to the lexical analyzer
    void set_input(std::string input);

    // get the result of the work
    const std::vector<Name> get_output() const;

    // get parenthesis balance
    const int get_parenth_count() const;


    // allow actions with the lexer
    friend class LexAction;
    friend class GoNext;
    friend class Push_GoNext;
    friend class Pop_GoNext;
    friend class GetTok;
    friend class GetTok_GoNext;
    friend class Push_GetTok_GoNext;
    friend class GetLeftPar;
    friend class GetRightPar;
};

#endif