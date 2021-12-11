#include <iostream>
#include <fstream>
#include "table.h"
#include "error.h"
#include "transition.h"
#include "evaluator.h"

void disp_tree(Node* root) {
    if (root) {
        std::cout << root->m_name->m_str << ": ";
        for (std::list<Node*>::iterator it = root->m_chld.begin(); it != root->m_chld.end(); it++)
            std::cout << (*it)->m_name->m_str << " ";
        std::cout << '\n';
        
        for (std::list<Node*>::iterator it = root->m_chld.begin(); it != root->m_chld.end(); it++)
            disp_tree(*it);
    }
    else {
        std::cout << "\n...empty tree...\n";
    }
}

int main() {
    set_tables();

    Lexer lexer;

    std::fstream fin;
    std::string str;

    char ch;
    // открываем файл в режиме чтени€
    //fin.open("C:\\p_folder\\lisp_program.txt");
    fin.open("~/Desktop/Sem_7/Compile/Lisp-interpreter-master/lisp_program.txt");
    // если открытие файла прошло корректно
    if (fin) {
        while (fin.get(ch)) {
            str.push_back(ch);
        }
        // закрытие потока
        fin.close();
    }
    // если открытие файла прошло некорректно
    else 
        std::cout << "NOT OPENED FILE" << std::endl;


    //std::cout << str << '\n';

    //std::string str2 = "(+ (quote 3 4) 2)";

    lexer.set_input(str);

    while (lexer.m_curr_state != State::TERM)
        lexer.do_transition();

    std::cout << "tokens:\n";
    for (const auto& p : lexer.m_out)
        std::cout << p.m_str << ":" << static_cast<int>(p.m_type) << " ";
    std::cout << "\nkeywords:\n";
    for (const auto& p : kword_table)
        for(auto pp = p; pp != nullptr; pp = pp -> m_next)
            if (pp) 
                std::cout << pp->m_str << " ";
    std::cout << "\nnames:\n";
    for (const auto& p : name_table)
        for (auto pp = p; pp != nullptr; pp = pp->m_next)
            if (pp)
                std::cout << pp->m_str << ":" << pp->m_level << " ";
    std::cout << "\nparenthesis count: " << lexer.m_parenth_count << "\n\n";

    Parser parser;
    parser.set_input(lexer.m_out);
    while (parser.m_curr_state != State::TERM)
        parser.do_transition();

    Evaluator ev = Evaluator(parser.m_root);
    ev.eval(parser.m_root);

    disp_tree(parser.m_root);

    if (!err_log.empty())
        std::cout << (--err_log.end())->m_descr;

    return 0;
}
