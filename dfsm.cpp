#include "dfsm.h"
#include "transition.h"

#define LETTERS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define DIGITS "0123456789"

DFSM::DFSM() : m_curr_state(State::INIT), m_curr_event(Event::NO_EVENT) {
    m_buffer.reserve(BUF_SIZE);
}

void DFSM::set_input(std::string input) {
    m_in = input;
    m_in.append(1, '\0');
    m_curr_char = m_in.begin();
    m_curr_event = m_event_table[*m_curr_char];
    m_out.reserve(m_in.size());
}

void DFSM::do_transition() {
    m_transit_table[m_curr_state][m_curr_event]->apply();
}

Preproc::Preproc() : DFSM() {
    using namespace std;

    //m_transit_table.insert(make_pair(, make_pair(, )));
}

Parser::Parser() : DFSM() {
    using namespace std;

    // create map from symbols to events
    for (unsigned char c = 0; c < 255; c++)
        m_event_table[c] = Event::SYM;
    for (const char& c : LETTERS)
        m_event_table[c] = Event::LETTER;
    for (const char& c : DIGITS)
        m_event_table[c] = Event::DIGIT;
    m_event_table['\0'] = Event::END;
    m_event_table['+'] =  Event::PLUS;
    m_event_table['-'] = Event::MINUS;
    m_event_table['/'] = Event::SLASH;
    m_event_table['*'] = Event::STAR;
    m_event_table['='] = Event::EQ;
    m_event_table['>'] = Event::GR;
    m_event_table['<'] = Event::LESS;
    m_event_table['e'] = Event::E_LETTER;
    m_event_table['E'] = Event::E_LETTER;
    m_event_table['"'] = Event::D_QUOT;
    m_event_table['.'] = Event::DOT;
    m_event_table['\\'] = Event::B_SLASH;
    m_event_table['_'] = Event::UN_SC;
    m_event_table[' '] = Event::SEP_SYM;
    m_event_table['\n'] = Event::SEP_SYM;
    m_event_table['\t'] = Event::SEP_SYM;

    // create table of transitions
    //// number parsing
    for (const auto& e : v_event)
        m_transit_table[State::INT_NUM][e] = new Transit_GetTok(this, State::INIT);
    m_transit_table[State::INT_NUM][Event::DIGIT] = new Transit_Wr_GoNext(this, State::INT_NUM);
    for (const auto& e : v_event)
        m_transit_table[State::FRAC_PART][e] = new Transit_GetTok(this, State::INIT);
    m_transit_table[State::FRAC_PART][Event::DIGIT] = new Transit_Wr_GoNext(this, State::FRAC_PART);
    m_transit_table[State::FRAC_PART][Event::E_LETTER] = new Transit_Wr_GoNext(this, State::EXP);
    for (const auto& e : v_event)
        m_transit_table[State::EXP][e] = new Transit_GetTok(this, State::INIT);
    m_transit_table[State::EXP][Event::DIGIT] = new Transit_Wr_GoNext(this, State::INT_NUM);
    m_transit_table[State::EXP][Event::PLUS] = new Transit_Wr_GoNext(this, State::INT_NUM);
    m_transit_table[State::EXP][Event::MINUS] = new Transit_Wr_GoNext(this, State::INT_NUM);
    for (const auto& e : v_event)
        m_transit_table[State::NUM][e] = new Transit_GetTok(this, State::INIT);
    m_transit_table[State::NUM][Event::DIGIT] = new Transit_Wr_GoNext(this, State::NUM);
    m_transit_table[State::NUM][Event::SLASH] = new Transit_Wr_GoNext(this, State::INT_NUM);
    m_transit_table[State::NUM][Event::DOT] = new Transit_Wr_GoNext(this, State::FRAC_PART);
    m_transit_table[State::NUM][Event::E_LETTER] = new Transit_Wr_GoNext(this, State::EXP);

    //// string parsing
    for (const auto& e : v_event)
        m_transit_table[State::ESC][e] = new Transit_Wr_GoNext(this, State::STR);
    for (const auto& e : v_event)
        m_transit_table[State::STR][e] = new Transit_Wr_GoNext(this, State::STR);
    m_transit_table[State::STR][Event::B_SLASH] = new Transit_GoNext(this, State::ESC);
    m_transit_table[State::STR][Event::D_QUOT] = new Transit_GetTok(this, State::INIT);

    //// identifier parsing
    for (const auto& e : v_event)
        m_transit_table[State::IDENT][e] = new Transit_GetTok(this, State::INIT);
    m_transit_table[State::IDENT][Event::LETTER] = new Transit_Wr_GoNext(this, State::IDENT);
    m_transit_table[State::IDENT][Event::E_LETTER] = new Transit_Wr_GoNext(this, State::IDENT);
    m_transit_table[State::IDENT][Event::DIGIT] = new Transit_Wr_GoNext(this, State::IDENT);
    m_transit_table[State::IDENT][Event::UN_SC] = new Transit_Wr_GoNext(this, State::IDENT);
    m_transit_table[State::IDENT][Event::MINUS] = new Transit_Wr_GoNext(this, State::IDENT);

    //// '+' parsing
    for (const auto& e : v_event)
        m_transit_table[State::PLUS][e] = new Transit_GetTok(this, State::INIT);
    m_transit_table[State::PLUS][Event::DIGIT] = new Transit_Wr_GoNext(this, State::NUM);

    //// '-' parsing
    for (const auto& e : v_event)
        m_transit_table[State::MINUS][e] = new Transit_GetTok(this, State::INIT);
    m_transit_table[State::MINUS][Event::DIGIT] = new Transit_Wr_GoNext(this, State::NUM);
    m_transit_table[State::MINUS][Event::LETTER] = new Transit_Wr_GoNext(this, State::IDENT);
    m_transit_table[State::MINUS][Event::E_LETTER] = new Transit_Wr_GoNext(this, State::IDENT);

    //// '>' parsing
    for (const auto& e : v_event)
        m_transit_table[State::GR][e] = new Transit_GetTok(this, State::INIT);
    m_transit_table[State::GR][Event::EQ] = new Transit_Wr_GetTok_GoNext(this, State::INIT);

    //// '<' parsing
    for (const auto& e : v_event)
        m_transit_table[State::LESS][e] = new Transit_GetTok(this, State::INIT);
    m_transit_table[State::LESS][Event::EQ] = new Transit_Wr_GetTok_GoNext(this, State::INIT);

    //// '/' parsing
    for (const auto& e : v_event)
        m_transit_table[State::SLASH][e] = new Transit_GetTok(this, State::INIT);
    m_transit_table[State::SLASH][Event::EQ] = new Transit_Wr_GetTok_GoNext(this, State::INIT);

    //// transition from 'INIT'
    for (const auto& e : v_event)
        m_transit_table[State::INIT][e] = new Transit_Wr_GetTok_GoNext(this, State::INIT);
    m_transit_table[State::INIT][Event::DIGIT] = new Transit_Wr_GoNext(this, State::NUM);
    m_transit_table[State::INIT][Event::D_QUOT] = new Transit_Wr_GoNext(this, State::STR);
    m_transit_table[State::INIT][Event::LETTER] = new Transit_Wr_GoNext(this, State::IDENT);
    m_transit_table[State::INIT][Event::E_LETTER] = new Transit_Wr_GoNext(this, State::IDENT);
    m_transit_table[State::INIT][Event::PLUS] = new Transit_Wr_GoNext(this, State::PLUS);
    m_transit_table[State::INIT][Event::MINUS] = new Transit_Wr_GoNext(this, State::MINUS);
    m_transit_table[State::INIT][Event::GR] = new Transit_Wr_GoNext(this, State::GR);
    m_transit_table[State::INIT][Event::LESS] = new Transit_Wr_GoNext(this, State::LESS);
    m_transit_table[State::INIT][Event::SLASH] = new Transit_Wr_GoNext(this, State::SLASH);
    m_transit_table[State::INIT][Event::SEP_SYM] = new Transit_GoNext(this, State::INIT);
    m_transit_table[State::INIT][Event::END] = new Transition(this, State::TERM);
}