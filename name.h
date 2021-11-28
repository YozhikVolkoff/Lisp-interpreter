#ifndef __TOKEN__
#define __TOKEN

#include <string>

enum class NameType {
    IDENT,
    NOT_DEFINED
};

class Name
{
public:
    std::string m_str;
    double m_value;
    NameType m_type;
    int m_level;
    Name* m_next;

    Name(std::string str = "name", NameType type = NameType::NOT_DEFINED, int level = 0)
        : m_str(str),
        m_value(0),
        m_type(type),
        m_level(level),
        m_next(nullptr) {}
};

#endif