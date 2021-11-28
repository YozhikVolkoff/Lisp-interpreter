#include <iostream>
#include "transition.h"

int main() {
    std::string str = "while not i do begin \n \
                           x : = cos(b[i]); !everewerv \n \
                           writeln(x); \n \
                           #|wecw cvw | sdv \n erec #|erve|# \
                           i: = i and (x >= 0); \n \
                       end";
    
    Lexer lexer;

    lexer.set_input(str);

    while (lexer.m_curr_state != State::TERM)
        lexer.do_transition();

    std::cout << "tokens:\n";
    for (const auto& p : lexer.m_out)
        std::cout << p.m_str << " ";
    std::cout << "\nkeywords:\n";
    for (const auto& p : lexer.m_kword_table)
        if (p) 
            std::cout << p->m_str << " ";
    std::cout << "\nnames:\n";
    for (const auto& p : lexer.m_name_table)
        if (p)
            std::cout << p->m_str << ":" << p->m_level << " ";
    std::cout << "\nparenthesis count: " << lexer.m_parenth_count;

    return 0;
}