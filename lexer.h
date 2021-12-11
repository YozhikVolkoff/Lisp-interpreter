#ifndef __LEXER__
#define __LEXER__

#include "dfsm.h"
#include "name.h"

class Lexer : public DFSM
{
public:
    std::map<char, Event> m_event_table;
    std::string m_in;
    std::string::iterator m_curr_char;
    std::string m_buffer;
    int m_parenth_count;
    std::vector<Name> m_out;

    Lexer();

    void set_input(std::string input);
};

#endif