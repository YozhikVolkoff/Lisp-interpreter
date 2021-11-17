#ifndef __TOKEN__
#define __TOKEN

#include <string>

enum class TokenType {
    NUM,
    IDENT,
    KEYWORD,
    OPERATOR
};

class Token
{
public:
    int m_value;
    TokenType m_type;

    Token(int value, TokenType type) : m_value(value), m_type(type) {}
};

#endif