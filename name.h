//-----------------------------------------------------------------------------
//  Author      : Sheptunov Dmitry
//  Description : A class for token storage
//-----------------------------------------------------------------------------


#ifndef __NAME__
#define __NAME__

#include <string>
#include <variant>

#define INIT_NAME " "
#define TERM_NAME "   "

// lexical type of the token
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

// type of the Lisp variable/constant
enum class ValueType {
    NUM,
    STR,
    BOOL,
    UNDEF
};

// stores value and type of the Lisp variable/constant
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
private:
    std::string m_str;
    Value m_value;
    NameType m_type;
    int m_level;
    Name* m_next;

public:
    Name(std::string str = "", NameType type = NameType::UNDEF, int level = 0)
        : m_str(str),
        m_type(type),
        m_level(level),
        m_next(nullptr) {}

    const std::string& get_str() const;
    const NameType& get_type() const;

    //void set_value(const Value& value);
    void set_value(const double& num);
    void set_value(const std::string& str);
    void set_value(const bool& b);

    const Value get_value() const;

    // see function in table.h
    friend Name* find_name(std::string str, const std::string& table_name, const int& ins,
        NameType type, int parenth_cnt);

};

#endif