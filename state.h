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

    PLUS,
    MINUS,
    GR,
    LESS,
    SLASH,

    // for comments
    SHARP,
    VERT_BAR,
    LINE_COMM,
    BLOCK_COMM,

    TERM
};

#endif