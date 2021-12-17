#include <cassert>
#include <iostream>
#include "table.h"
#include "error.h"
#include "transition.h"
#include "evaluator.h"
#include "test.h"

void Name_test() {
    set_tables();

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

void general_test() {
    // this is an example, further I suggest one of possible ways
    set_tables();
    std::string line = "(+ 1 2)";  // you can vary this
    double result = 0.0;           // and this
    double expected_res = 3.0;     // and this

    Lexer lexer;
    lexer.set_input(line);
    while (lexer.m_curr_state != State::TERM)
        lexer.do_transition();
    if (lexer.get_parenth_count() == 0) {
        Parser parser;
        parser.set_input(lexer.get_output());
        while (parser.m_curr_state != State::TERM)
            parser.do_transition();
        if (err_log.empty()) {
            Evaluator ev = Evaluator(parser.get_output());
            result = std::get<double>(ev.eval(parser.get_output())->val);  // saving the result
        }
    }
    else {
        err_log.push_back(ErrorMessage("parenthesis count mismatch"));
    }

    assert(result == expected_res);   // check

    // also you can check the error log

    err_log.clear();
}