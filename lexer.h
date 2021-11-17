#ifndef __LEXER__
#define __LEXER__

#include<vector>
#include "token.h"
#include "transition.h"

enum class State {
    INIT,
    TERM,
    LAST_ONE
};

enum class Event {
    DIGIT,
    LETTER,
    LAST_ONE
};

class Lexer
{
private:
    std::string m_buffer;
    char m_curr_char;
    State m_curr_state;
    std::vector<std::vector<Transition> > m_transit_table;
    std::string m_input;
    std::vector<Token> m_output;

public:
    Lexer()
        : m_curr_state(State::INIT),
        m_transit_table(static_cast<int>(State::LAST_ONE), 
            std::vector<Transition>(static_cast<int>(Event::LAST_ONE))) {}

    Lexer(const std::string& input)
        : m_curr_state(State::INIT),
        m_transit_table(static_cast<int>(State::LAST_ONE),
            std::vector<Transition>(static_cast<int>(Event::LAST_ONE))),
        m_input(input) {}

    void set_input(const std::string& input) { m_input = input; }

    std::vector<Token> get_output() const { return m_output; }
    
    int do_transition() {
        int event = static_cast<int>(to_event(m_curr_char));
        Transition transit = m_transit_table[static_cast<int>(m_curr_state)][event];

        if (this != transit.m_action.m_lexer) {
            m_curr_state = State::TERM;
            return -1;
        }

        transit.m_action.apply();

        m_curr_state = transit.m_target_state;

        return 0;
    }

    int add_transition(const State& s_state, const Event& event, const Action& action, const State& t_state) {
        m_transit_table[static_cast<int>(s_state)][static_cast<int>(event)] = Transition(action, t_state);

        return 0;
    }

    friend class Action;
};

int to_event(int i) {
    return 0;
}
#endif