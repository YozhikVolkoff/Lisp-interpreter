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
    std::string m_value;
    TokenType m_type;

    Token(std::string s_token) : m_value(s_token), m_type(TokenType::IDENT) {}
};

#endif