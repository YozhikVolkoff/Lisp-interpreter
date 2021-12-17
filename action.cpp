#include "action.h"
#include "table.h"
#include "error.h"

std::vector<Name*> kword_table;
std::vector<Name*> name_table;
std::list<ErrorMessage> err_log;

void GoNext::apply() {
    // go to the next symbol
    m_lexer->m_curr_char++;
    // transform symbol to the element of automaton alphabed
    m_lexer->m_curr_event = m_lexer->m_event_table[*m_lexer->m_curr_char];
}

void Push_GoNext::apply() {
    // put current symbol to the buffer
    m_lexer->m_buffer.push_back(*m_lexer->m_curr_char);
    GoNext::apply();
}

void Pop_GoNext::apply() {
    // extract last symbol from the buffer
    m_lexer->m_buffer.pop_back();
    GoNext::apply();
}

void GetTok::apply() {
    // get string from buffer
    std::string str = std::string(m_lexer->m_buffer);
    if (m_name_type == NameType::IDENT)
        // check if this is not the keyword
        if (find_name(str, "kword", 0) == nullptr)
            find_name(str, "name", 1, m_name_type, m_lexer->m_parenth_count);
    
    // put token to the output
    m_lexer->m_out.push_back(Name(str, m_name_type, m_lexer->m_parenth_count));
    // free buffer
    m_lexer->m_buffer.clear();
}

void GetTok_GoNext::apply() {
    GetTok::apply();
    m_lexer->m_curr_char++;
    m_lexer->m_curr_event = m_lexer->m_event_table[*m_lexer->m_curr_char];
}

void Push_GetTok_GoNext::apply() {
    m_lexer->m_buffer.push_back(*m_lexer->m_curr_char);
    GetTok::apply();
    m_lexer->m_curr_char++;
    m_lexer->m_curr_event = m_lexer->m_event_table[*m_lexer->m_curr_char];
}

void GetLeftPar::apply() {
    Push_GetTok_GoNext::apply();
    // increase parenthesis counter
    m_lexer->m_parenth_count++;
}

void GetRightPar::apply() {
    Push_GetTok_GoNext::apply();
    // decrease parenthesis counter
    m_lexer->m_parenth_count--;
}

void AddArg::apply() {
    Node* curr_node = m_parser->m_curr_node;
    if (curr_node) {
        Name* name = find_name(m_parser->m_curr_name->get_str(), "name", 0);
        // check if the current token is the variable
        if (name) {
            // add the child node
            curr_node->m_chld.push_back(new Node(name, curr_node));

            // go to the neext iteration
            m_parser->m_curr_name++;
            m_parser->m_curr_event = m_parser->to_event(*m_parser->m_curr_name);
        }
        // check if the current token is the constant number or string
        else if (m_parser->m_curr_name->get_type() == NameType::NUM ||
            m_parser->m_curr_name->get_type() == NameType::STR) {
            curr_node->m_chld.push_back(new Node(new Name(*m_parser->m_curr_name), curr_node));

            // go to the next iteration
            m_parser->m_curr_name++;
            m_parser->m_curr_event = m_parser->to_event(*m_parser->m_curr_name);
        }
        else {
            // argument can only be variable or constant
            err_log.push_back(ErrorMessage(m_parser->m_curr_name->get_str() + " is not variable or const"));
            m_parser->m_curr_event = Event::END;
        }
    }
    else {
        // exception
        err_log.push_back(ErrorMessage("Unexpected value of m_curr_node: nullptr"));
        m_parser->m_curr_event = Event::END;
    }
}

void AddOp::apply() {
    Node* curr_node = m_parser->m_curr_node;
    if (curr_node) {
        Name* kword = find_name(m_parser->m_curr_name->get_str(), "kword", 0);
        // check if the current token is the keyword
        if (kword) {
            Node* chld = new Node(kword, curr_node);
            // add the child node
            curr_node->m_chld.push_back(chld);
            // go to this node
            m_parser->m_curr_node = chld;

            // go to the next iteration
            m_parser->m_curr_name++;
            m_parser->m_curr_event = m_parser->to_event(*m_parser->m_curr_name);
        }
        else {
            // operator is always a keyword
            err_log.push_back(ErrorMessage(m_parser->m_curr_name->get_str() + " is not a function or operator name"));
            m_parser->m_curr_event = Event::END;
        }
    }
    else {
        // exception
        err_log.push_back(ErrorMessage("Unexpected value of m_curr_node: nullptr"));
        m_parser->m_curr_event = Event::END;
    }
}

void Skip_AddOp::apply() {
    // skip one token
    m_parser->m_curr_name++;
    m_parser->m_curr_event = m_parser->to_event(*m_parser->m_curr_name);
    // add an operation
    AddOp::apply();
}

void GoToParent::apply() {
    if (m_parser->m_curr_node) {
        Node* parent = m_parser->m_curr_node->m_parent;
        // check if there is a parent
        if (parent) {
            // go to the parent
            m_parser->m_curr_node = parent;

            // go to the next iteration
            m_parser->m_curr_name++;
            m_parser->m_curr_event = m_parser->to_event(*m_parser->m_curr_name);
        }
        else {
            // exception
            err_log.push_back(ErrorMessage("Unexpected value of m_parent: nullptr"));
            m_parser->m_curr_event = Event::END;
        }
    }
    else {
        // exception
        err_log.push_back(ErrorMessage("Unexpected value of m_curr_node: nullptr"));
        m_parser->m_curr_event = Event::END;
    }
}

void Err_UndefName::apply() {
    // undefined token
    err_log.push_back(ErrorMessage(m_parser->m_curr_name->get_str() + ": undefined token")); 
    m_parser->m_curr_event = Event::END;
}