#include <iostream>
#include "table.h"
#include "error.h"
#include "transition.h"
#include "evaluator.h"
#include "test.h"

void interpreter() {
    set_tables();

    while (true) {
        std::string line;
        getline(std::cin, line);

        if (line == ":exit")
            break;
        else {
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
                    ev.eval(parser.get_output());
                }
            }
            else {
                err_log.push_back(ErrorMessage("parenthesis count mismatch"));
            }

            if(!err_log.empty()) {
                std::cout << err_log.begin()->m_descr;
                err_log.clear();
            }
        }
    }
}

int main() {
    interpreter();

    //Name_test();
    //general_test(); 

    return 0;
}
