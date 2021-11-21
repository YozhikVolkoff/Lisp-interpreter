#include <iostream>
#include "transition.h"

std::vector<Token> parse(std::string input) {
    Parser parser;

    parser.set_input(input);

    while (parser.m_curr_state != State::TERM)
        parser.do_transition();

    return parser.m_out;
}

int main() {
    std::string str = "while not i do begin\
                           x : = cos(b[i]);\
                           writeln(x);\
                           i: = i and (x >= 0);\
                       end";
    
    std::vector<Token> out = parse(str);
    for (const auto& p : out)
        std::cout << p.m_value << " ";

    return 0;
}