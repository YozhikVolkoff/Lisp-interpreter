#ifndef __EVENT__
#define __EVENT__

enum class Event
{
    // any symbol except mentioned
    SYM,

    // any letter except mentioned
    LETTER,

    // separation symbol
    SEP_SYM,

    // any digit
    DIGIT,

    // for exponenta
    E_LETTER,

    // math
    PLUS,
    MINUS,
    SLASH,
    STAR,
    EQ,
    GR,
    LESS,

    // special
    D_QUOT,
    DOT,
    B_SLASH,
    UN_SC,
    END,

    NO_EVENT
};

const std::vector<Event> v_event{Event::SYM, Event::LETTER, Event::SEP_SYM,
                                 Event::DIGIT, Event::E_LETTER, Event::PLUS,
                                 Event::MINUS, Event::SLASH, Event::STAR,
                                 Event::EQ, Event::GR, Event::LESS,
                                 Event::D_QUOT, Event::DOT, Event::B_SLASH, 
                                 Event::UN_SC, Event::END};

#endif