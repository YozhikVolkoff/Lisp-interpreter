#include <cassert>
#include "test.h"
#include "name.h"

void Name_test() {
    Name name("name");
    name.set_value(true);
    assert(std::get<bool>(name.get_value().val) == true);
    assert(name.get_value().type == ValueType::BOOL);

    name = Name("1", NameType::NUM);
    assert(std::get<double>(name.get_value().val) == 1);
    assert(name.get_value().type == ValueType::NUM);

    name = Name("t", NameType::IDENT);
    assert(std::get<bool>(name.get_value().val) == true);
    assert(name.get_value().type == ValueType::BOOL);

    name = Name("1", NameType::NUM);
    assert(std::get<double>(name.get_value().val) == 1);
    assert(name.get_value().type == ValueType::NUM);

    name = Name("1", NameType::STR);
    assert(std::get<std::string>(name.get_value().val) == "1");
    assert(name.get_value().type == ValueType::STR);
}