#include "lexer.h"
#include "transition.h"

#define BUF_SIZE

#define LETTERS "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define DIGITS "0123456789"

void Lexer::set_input(std::string input) {
    m_in = input;
    // add the terminating symbol
    m_in.append(1, '\0');
    // set the current symbol to the beginning
    m_curr_char = m_in.begin();
    // map the current symbol to an event
    m_curr_event = m_event_table[*m_curr_char];
    // allocate memory for the output
    m_out.reserve(m_in.size());
}

const std::vector<Name> Lexer::get_output() const {
    return m_out;
}

const int Lexer::get_parenth_count() const {
    return m_parenth_count;
}

Lexer::Lexer()
    : DFSM(),
    m_parenth_count(0)
{
    using namespace std;

    // allocate memory for the buffer
    m_buffer.reserve(BUF_SIZE);

    // create map from symbols to events
    for (unsigned char c = 0; c < 255; c++)
        m_event_table[c] = Event::SYM;
    for (const char& c : LETTERS)
        m_event_table[c] = Event::LETTER;
    for (const char& c : DIGITS)
        m_event_table[c] = Event::DIGIT;
    m_event_table['\0'] = Event::END;
    m_event_table['+'] = Event::PLUS;
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
    m_transit_table[State::STR][Event::D_QUOT] = new Transition(new GetTok_GoNext(this, NameType::STR), State::INIT);

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

    //// transitions from 'INIT'
    for (const auto& e : lex_events)
        m_transit_table[State::INIT][e] = new Transition(new Push_GetTok_GoNext(this), State::INIT);
    m_transit_table[State::INIT][Event::DIGIT] = new Transition(new Push_GoNext(this), State::NUM);
    m_transit_table[State::INIT][Event::D_QUOT] = new Transition(new GoNext(this), State::STR);
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