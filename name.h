#ifndef __NAME__
#define __NAME__

#include <string>

#define INIT_NAME " "
#define TERM_NAME "   "

enum class NameType {
    INIT,
    TERM,
    IDENT,
    NUM,
    STR,
    OP,
    LEFT_PAR,
    RIGHT_PAR,
    UNDEF
};

class Name
{
public:
    std::string m_str;
    double m_value;
    NameType m_type;
    int m_level;
    Name* m_next;

    Name(std::string str = "", NameType type = NameType::UNDEF, int level = 0)
        : m_str(str),
        m_value(0),
        m_type(type),
        m_level(level),
        m_next(nullptr) {}
};

#endif