#ifndef __NAME__
#define __NAME__

#include <string>
#include <variant>

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

enum class ValueType {
    NUM,
    STR,
    BOOL,
    UNDEF
};

struct Value {
    std::variant<double, std::string, bool> val;
    ValueType type;

    Value() : val(0.0), type(ValueType::UNDEF) {}
    Value(const double& num) : val(num), type(ValueType::NUM) {}
    Value(const std::string& str) : val(str), type(ValueType::STR) {}
    Value(const bool& b) : val(b), type(ValueType::BOOL) {}
};

class Name
{
public:
    std::string m_str;
    Value m_value;
    NameType m_type;
    int m_level;
    Name* m_next;

    Name(std::string str = "", NameType type = NameType::UNDEF, int level = 0)
        : m_str(str),
        m_type(type),
        m_level(level),
        m_next(nullptr) {}

    void set_value(const double& num);
    void set_value(const std::string& str);
    void set_value(const bool& b);

    const Value get_value() const;
};

#endif