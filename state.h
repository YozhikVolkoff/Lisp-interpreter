//-----------------------------------------------------------------------------
//  Author      : Sheptunov Dmitry
//  Description : State-machine states
//-----------------------------------------------------------------------------

#ifndef __STATE__
#define __STATE__

enum class State
{
    // initial and terminal states
    INIT,
    TERM,

    // lexer states
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
    // for multi-character operator parsing
    PLUS,
    MINUS,
    GR,
    LESS,
    SLASH,
    // for comments
    SHARP,
    VERT_BAR,
    LINE_COMM,
    BLOCK_COMM
};

#endif