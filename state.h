#ifndef __STATE__
#define __STATE__

enum class State
{
    INIT,

    // for number parsing
    NUM,
    INT_NUM,
    FRAC_PART,
    EXP,

    // for string parsing
    STR,
    ESC,

    // for identifier parsing
    IDENT,

    TERM
};

#endif