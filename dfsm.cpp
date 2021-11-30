#include "transition.h"

#define LETTERS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define DIGITS "0123456789"

DFSM::DFSM() 
    : m_curr_state(State::INIT), 
    m_curr_event(Event::NO_EVENT), 
    m_curr_transit(nullptr) {}

void DFSM::do_transition() {
    m_curr_transit = m_transit_table[m_curr_state][m_curr_event];
    m_curr_transit->m_action->apply();
    m_curr_state = m_curr_transit->m_target_state;
}

void Lexer::set_input(std::string input) {
    m_in = input;
    m_in.append(1, '\0');
    m_curr_char = m_in.begin();
    m_curr_event = m_event_table[*m_curr_char];
    m_out.reserve(m_in.size());
}

Lexer::Lexer()
    : DFSM(),
    m_parenth_count(0)
{
    using namespace std;

    m_buffer.reserve(BUF_SIZE);

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
    m_event_table['\t'] = Event::SEP_SYM;
    m_event_table['\n'] = Event::ENDLINE;
    m_event_table['#'] = Event::SHARP;
    m_event_table['!'] = Event::EXCL;
    m_event_table['|'] = Event::VERT_BAR;
    m_event_table['('] = Event::LEFT_PAR;
    m_event_table[')'] = Event::RIGHT_PAR;

    // create table of transitions
    //// single-line comment parsing
    for (const auto& e : lex_events)
        m_transit_table[State::LINE_COMM][e] = new Transition(new GoNext(this), State::LINE_COMM);
    m_transit_table[State::LINE_COMM][Event::ENDLINE] = new Transition(new GoNext(this), State::INIT);

    //// block-comment parsing
    for (const auto& e : lex_events)
        m_transit_table[State::SHARP][e] = new Transition(new GetTok(this), State::INIT);
    m_transit_table[State::SHARP][Event::VERT_BAR] = new Transition(new Pop_GoNext(this), State::BLOCK_COMM);
    for (const auto& e : lex_events)
        m_transit_table[State::BLOCK_COMM][e] = new Transition(new GoNext(this), State::BLOCK_COMM);
    m_transit_table[State::BLOCK_COMM][Event::VERT_BAR] = new Transition(new GoNext(this), State::VERT_BAR);
    for (const auto& e : lex_events)
        m_transit_table[State::VERT_BAR][e] = new Transition(new GoNext(this), State::BLOCK_COMM);
    m_transit_table[State::VERT_BAR][Event::SHARP] = new Transition(new GoNext(this), State::INIT);

    //// number parsing
    for (const auto& e : lex_events)
        m_transit_table[State::INT_NUM][e] = new Transition(new GetTok(this, NameType::NUM), State::INIT);
    m_transit_table[State::INT_NUM][Event::DIGIT] = new Transition(new Push_GoNext(this), State::INT_NUM);
    for (const auto& e : lex_events)
        m_transit_table[State::FRAC_PART][e] = new Transition(new GetTok(this, NameType::NUM), State::INIT);
    m_transit_table[State::FRAC_PART][Event::DIGIT] = new Transition(new Push_GoNext(this), State::FRAC_PART);
    m_transit_table[State::FRAC_PART][Event::E_LETTER] = new Transition(new Push_GoNext(this), State::EXP);
    for (const auto& e : lex_events)
        m_transit_table[State::EXP][e] = new Transition(new GetTok(this, NameType::NUM), State::INIT);
    m_transit_table[State::EXP][Event::DIGIT] = new Transition(new Push_GoNext(this), State::INT_NUM);
    m_transit_table[State::EXP][Event::PLUS] = new Transition(new Push_GoNext(this), State::INT_NUM);
    m_transit_table[State::EXP][Event::MINUS] = new Transition(new Push_GoNext(this), State::INT_NUM);
    for (const auto& e : lex_events)
        m_transit_table[State::NUM][e] = new Transition(new GetTok(this, NameType::NUM), State::INIT);
    m_transit_table[State::NUM][Event::DIGIT] = new Transition(new Push_GoNext(this), State::NUM);
    m_transit_table[State::NUM][Event::SLASH] = new Transition(new Push_GoNext(this), State::INT_NUM);
    m_transit_table[State::NUM][Event::DOT] = new Transition(new Push_GoNext(this), State::FRAC_PART);
    m_transit_table[State::NUM][Event::E_LETTER] = new Transition(new Push_GoNext(this), State::EXP);

    //// string parsing
    for (const auto& e : lex_events)
        m_transit_table[State::ESC][e] = new Transition(new Push_GoNext(this), State::STR);
    for (const auto& e : lex_events)
        m_transit_table[State::STR][e] = new Transition(new Push_GoNext(this), State::STR);
    m_transit_table[State::STR][Event::B_SLASH] = new Transition(new GoNext(this), State::ESC);
    m_transit_table[State::STR][Event::D_QUOT] = new Transition(new GetTok(this, NameType::STR), State::INIT);

    //// identifier parsing
    for (const auto& e : lex_events)
        m_transit_table[State::IDENT][e] = new Transition(new GetTok(this, NameType::IDENT), State::INIT);
    m_transit_table[State::IDENT][Event::LETTER] = new Transition(new Push_GoNext(this), State::IDENT);
    m_transit_table[State::IDENT][Event::E_LETTER] = new Transition(new Push_GoNext(this), State::IDENT);
    m_transit_table[State::IDENT][Event::DIGIT] = new Transition(new Push_GoNext(this), State::IDENT);
    m_transit_table[State::IDENT][Event::UN_SC] = new Transition(new Push_GoNext(this), State::IDENT);
    m_transit_table[State::IDENT][Event::MINUS] = new Transition(new Push_GoNext(this), State::IDENT);

    //// '+' parsing
    for (const auto& e : lex_events)
        m_transit_table[State::PLUS][e] = new Transition(new GetTok(this, NameType::OP), State::INIT);
    m_transit_table[State::PLUS][Event::DIGIT] = new Transition(new Push_GoNext(this), State::NUM);

    //// '-' parsing
    for (const auto& e : lex_events)
        m_transit_table[State::MINUS][e] = new Transition(new GetTok(this, NameType::OP), State::INIT);
    m_transit_table[State::MINUS][Event::DIGIT] = new Transition(new Push_GoNext(this), State::NUM);
    m_transit_table[State::MINUS][Event::LETTER] = new Transition(new Push_GoNext(this), State::IDENT);
    m_transit_table[State::MINUS][Event::E_LETTER] = new Transition(new Push_GoNext(this), State::IDENT);

    //// '>' parsing
    for (const auto& e : lex_events)
        m_transit_table[State::GR][e] = new Transition(new GetTok(this, NameType::OP), State::INIT);
    m_transit_table[State::GR][Event::EQ] = new Transition(new Push_GetTok_GoNext(this, NameType::OP), State::INIT);

    //// '<' parsing
    for (const auto& e : lex_events)
        m_transit_table[State::LESS][e] = new Transition(new GetTok(this, NameType::OP), State::INIT);
    m_transit_table[State::LESS][Event::EQ] = new Transition(new Push_GetTok_GoNext(this, NameType::OP), State::INIT);

    //// '/' parsing
    for (const auto& e : lex_events)
        m_transit_table[State::SLASH][e] = new Transition(new GetTok(this, NameType::OP), State::INIT);
    m_transit_table[State::SLASH][Event::EQ] = new Transition(new Push_GetTok_GoNext(this, NameType::OP), State::INIT);

    //// transition from 'INIT'
    for (const auto& e : lex_events)
        m_transit_table[State::INIT][e] = new Transition(new Push_GetTok_GoNext(this), State::INIT);
    m_transit_table[State::INIT][Event::DIGIT] = new Transition(new Push_GoNext(this), State::NUM);
    m_transit_table[State::INIT][Event::D_QUOT] = new Transition(new Push_GoNext(this), State::STR);
    m_transit_table[State::INIT][Event::LETTER] = new Transition(new Push_GoNext(this), State::IDENT);
    m_transit_table[State::INIT][Event::E_LETTER] = new Transition(new Push_GoNext(this), State::IDENT);
    m_transit_table[State::INIT][Event::PLUS] = new Transition(new Push_GoNext(this), State::PLUS);
    m_transit_table[State::INIT][Event::MINUS] = new Transition(new Push_GoNext(this), State::MINUS);
    m_transit_table[State::INIT][Event::GR] = new Transition(new Push_GoNext(this), State::GR);
    m_transit_table[State::INIT][Event::LESS] = new Transition(new Push_GoNext(this), State::LESS);
    m_transit_table[State::INIT][Event::SLASH] = new Transition(new Push_GoNext(this), State::SLASH);
    m_transit_table[State::INIT][Event::EXCL] = new Transition(new GoNext(this), State::LINE_COMM);
    m_transit_table[State::INIT][Event::SHARP] = new Transition(new Push_GoNext(this), State::SHARP);
    m_transit_table[State::INIT][Event::SEP_SYM] = new Transition(new GoNext(this), State::INIT);
    m_transit_table[State::INIT][Event::ENDLINE] = new Transition(new GoNext(this), State::INIT);
    m_transit_table[State::INIT][Event::LEFT_PAR] = new Transition(new GetLeftPar(this, NameType::LEFT_PAR), State::INIT);
    m_transit_table[State::INIT][Event::RIGHT_PAR] = new Transition(new GetRightPar(this, NameType::RIGHT_PAR), State::INIT);
    m_transit_table[State::INIT][Event::END] = new Transition(new NoAction(), State::TERM);
}

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