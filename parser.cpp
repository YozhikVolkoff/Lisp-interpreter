#include "parser.h"
#include "transition.h"

void Parser::set_input(std::vector<Name> input) {
    m_in = input;
    m_in.push_back(Name(TERM_NAME, NameType::TERM));
    m_curr_name = m_in.begin();
    m_curr_event = to_event(*m_curr_name);
}

Event Parser::to_event(const Name& name) {
    switch (name.m_type) {
    case NameType::LEFT_PAR:
        return Event::LEFT_PAR;
        break;
    case NameType::RIGHT_PAR:
        return Event::RIGHT_PAR;
        break;
    case NameType::IDENT:
        return Event::IDENT;
        break;
    case NameType::NUM:
        return Event::NUM;
        break;
    case NameType::STR:
        return Event::STR;
        break;
    case NameType::TERM:
        return Event::END;
        break;
    default:
        return Event::UNDEF;
        break;
    }
}

Parser::Parser()
    : DFSM(),
    m_root(new Node(new Name(INIT_NAME, NameType::INIT)))
{
    m_curr_node = m_root;

    // create transition table
    m_transit_table[State::INIT][Event::LEFT_PAR] = new Transition(new Skip_AddOp(this), State::INIT);
    m_transit_table[State::INIT][Event::RIGHT_PAR] = new Transition(new GoToParent(this), State::INIT);
    m_transit_table[State::INIT][Event::IDENT] = new Transition(new AddArg(this), State::INIT);
    m_transit_table[State::INIT][Event::NUM] = new Transition(new AddArg(this), State::INIT);
    m_transit_table[State::INIT][Event::STR] = new Transition(new AddArg(this), State::INIT);
    m_transit_table[State::INIT][Event::UNDEF] = new Transition(new Err_UndefName(this), State::INIT);
    m_transit_table[State::INIT][Event::END] = new Transition(new NoAction(), State::TERM);
}