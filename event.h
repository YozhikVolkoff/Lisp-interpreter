#ifndef __EVENT__
#define __EVENT__

enum class Event
{
    /* // any symbol except mentioned
    SYM,

    // any letter except mentioned
    LETTER,

    // separation symbol
    SEP_SYM, */

    // any digit
    DIGIT,

    // for exponenta
    E_LETTER,

    // math
    PLUS,
    MINUS,
    SLASH,
    STAR,
    EQUAL,
    MORE,
    LESS,

    // special
    LEFT_PAR,
    RIGHT_PAR,
    SINGLE_Q,
    DOUBLE_Q,
    DOT,
    B_SLASH,
    UNDERSCORE,
    DASH,

    NO_EVENT
};

#endif