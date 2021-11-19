#include "dfsm.h"

using namespace std;

void DFSM::set_input(string input) {
    m_input = input;
    m_curr_char = input.begin();
}

void DFSM::do_transition() {
    if (this == m_transit_table[m_curr_state][m_curr_event].m_action.m_dfsm) {
        m_transit_table[m_curr_state][m_curr_event].m_action.apply();
        m_curr_state = m_transit_table[m_curr_state][m_curr_event].m_target_state;
    }
}

Preproc::Preproc() : DFSM() {
    //m_transit_table.insert(make_pair(, make_pair(, )));
}

Parser::Parser() : DFSM() {
    // number parsing
    //m_transit_table.insert(make_pair(State::INT_NUM, make_pair(Event::DIGIT, Transition(<action>, State::INT_NUM))));
    //  m_transit_table.insert(make_pair(State::INT_NUM, make_pair(Event::<else>, Transition(<action>, State::INIT))));
    //m_transit_table.insert(make_pair(State::FRAC_PART, make_pair(Event::DIGIT, Transition(<action>, State::FRAC_PART))));
    //m_transit_table.insert(make_pair(State::FRAC_PART, make_pair(Event::E_LETTER, Transition(<action>, State::EXP))));
    //  m_transit_table.insert(make_pair(State::FRAC_PART, make_pair(Event::<else>, Transition(<action>, State::INIT))));
    //m_transit_table.insert(make_pair(State::EXP, make_pair(Event::E_LETTER, Transition(<action>, State::INT_NUM))));
    //m_transit_table.insert(make_pair(State::EXP, make_pair(Event::PLUS, Transition(<action>, State::INT_NUM))));
    //m_transit_table.insert(make_pair(State::EXP, make_pair(Event::MINUS, Transition(<action>, State::INT_NUM))));
    //  m_transit_table.insert(make_pair(State::EXP, make_pair(Event::<else>, Transition(<action>, State::INIT))));
    //m_transit_table.insert(make_pair(State::NUM, make_pair(Event::DIGIT, Transition(<action>, State::NUM))));
    //m_transit_table.insert(make_pair(State::NUM, make_pair(Event::SLASH, Transition(<action>, State::INT_NUM))));
    //m_transit_table.insert(make_pair(State::NUM, make_pair(Event::DOT, Transition(<action>, State::FRAC_PART))));
    //m_transit_table.insert(make_pair(State::NUM, make_pair(Event::E_LETTER, Transition(<action>, State::EXP))));
    //  m_transit_table.insert(make_pair(State::NUM, make_pair(Event::<else>, Transition(<action>, State::INIT))));

    // string parsing
    //m_transit_table.insert(make_pair(State::ESC, make_pair(Event::<any_symbol>, Transition(<action>, State::STR))));
    //m_transit_table.insert(make_pair(State::STR, make_pair(Event::B_SLASH, Transition(<action>, State::ESC))));
    //m_transit_table.insert(make_pair(State::STR, make_pair(Event::DOUBLE_Q, Transition(<action>, State::INIT))));
    //  m_transit_table.insert(make_pair(State::STR, make_pair(Event::<else>, Transition(<action>, State::STR))));

    // identifier parsing
    //m_transit_table.insert(make_pair(State::IDENT, make_pair(Event::<any_letter>, Transition(<action>, State::IDENT))));
    //m_transit_table.insert(make_pair(State::IDENT, make_pair(Event::DIGIT, Transition(<action>, State::IDENT))));
    //m_transit_table.insert(make_pair(State::IDENT, make_pair(Event::UNDERSCORE, Transition(<action>, State::IDENT))));
    //m_transit_table.insert(make_pair(State::IDENT, make_pair(Event::DASH, Transition(<action>, State::IDENT))));
    //  m_transit_table.insert(make_pair(State::IDENT, make_pair(Event::<else>, Transition(<action>, State::INIT))));
}