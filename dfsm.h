#ifndef __DFSM__
#define __DFSM__

#define BUF_SIZE 512

#include<vector>
#include <map>
#include <cassert>
#include "name.h"
#include "event.h"
#include "state.h"

class Transition;

class DFSM
{
public:
    State m_curr_state;
    Event m_curr_event;
    Transition* m_curr_transit;
    std::map<State, std::map<Event, Transition*> > m_transit_table;

    DFSM();

    void do_transition();
};

class Lexer : public DFSM
{
public:
    std::map<char, Event> m_event_table;
    std::string m_in;
    std::string::iterator m_curr_char;
    std::string m_buffer;
    int m_parenth_count;
    std::vector<Name> m_out;
    std::vector<Name*> m_kword_table;
    const int kwords_num = 128;
    std::vector<Name*> m_name_table;
    const int names_num = 512;

    Lexer();

    void set_input(std::string input);

    int find_name(std::string str, const std::string& table_name, const int& ins);
};

#endif