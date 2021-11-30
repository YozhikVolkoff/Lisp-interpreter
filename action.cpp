#include "action.h"
#include "table.h"
#include "error.h"

std::vector<Name*> kword_table;
std::vector<Name*> name_table;
std::list<ErrorMessage> err_log;

void GoNext::apply() {
    m_lexer->m_curr_char++;
    m_lexer->m_curr_event = m_lexer->m_event_table[*m_lexer->m_curr_char];
}

void Push_GoNext::apply() {
    m_lexer->m_buffer.push_back(*m_lexer->m_curr_char);
    m_lexer->m_curr_char++;
    m_lexer->m_curr_event = m_lexer->m_event_table[*m_lexer->m_curr_char];
}

void Pop_GoNext::apply() {
    m_lexer->m_buffer.pop_back();
    m_lexer->m_curr_char++;
    m_lexer->m_curr_event = m_lexer->m_event_table[*m_lexer->m_curr_char];
}

void GetTok::apply() {
    std::string buffer = std::string(m_lexer->m_buffer);
    if (m_name_type == NameType::IDENT)
        if (find_name(buffer, "kword", 0) == nullptr)
            find_name(buffer, "name", 1, m_name_type, m_lexer->m_parenth_count);
    
    m_lexer->m_out.push_back(Name(buffer, m_name_type, m_lexer->m_parenth_count));
    m_lexer->m_buffer.clear();
}

void Push_GetTok_GoNext::apply() {
    m_lexer->m_buffer.push_back(*m_lexer->m_curr_char);
    GetTok::apply();
    m_lexer->m_curr_char++;
    m_lexer->m_curr_event = m_lexer->m_event_table[*m_lexer->m_curr_char];
}

void GetLeftPar::apply() {
    Push_GetTok_GoNext::apply();
    m_lexer->m_parenth_count++;
}

void GetRightPar::apply() {
    Push_GetTok_GoNext::apply();
    m_lexer->m_parenth_count--;
}

void AddArg::apply() {
    Node* curr_node = m_parser->m_curr_node;
    if (curr_node) {
        Name* name = find_name(m_parser->m_curr_name->m_str, "name", 0);
        if (name) {
            curr_node->m_chld.push_back(new Node(name, curr_node));

            m_parser->m_curr_name++;
            m_parser->m_curr_event = m_parser->to_event(*m_parser->m_curr_name);
        }
        else if (m_parser->m_curr_name->m_type == NameType::NUM ||
            m_parser->m_curr_name->m_type == NameType::STR) {
            curr_node->m_chld.push_back(new Node(new Name(*m_parser->m_curr_name), curr_node));

            m_parser->m_curr_name++;
            m_parser->m_curr_event = m_parser->to_event(*m_parser->m_curr_name);
        }
        else {
            err_log.push_back(ErrorMessage("Not variable or const"));    // todo: err_message
            m_parser->m_curr_event = Event::END;
        }
    }
    else {
        err_log.push_back(ErrorMessage("Unexpected value of m_curr_node: nullptr"));    // todo: err_message
        m_parser->m_curr_event = Event::END;
    }
}

void AddOp::apply() {
    Node* curr_node = m_parser->m_curr_node;
    if (curr_node) {
        Name* kword = find_name(m_parser->m_curr_name->m_str, "kword", 0);
        if (kword) {
            Node* chld = new Node(kword, curr_node);
            curr_node->m_chld.push_back(chld);
            m_parser->m_curr_node = chld;

            m_parser->m_curr_name++;
            m_parser->m_curr_event = m_parser->to_event(*m_parser->m_curr_name);
        }
        else {
            err_log.push_back(ErrorMessage("Not a function name"));    // todo: err_message
            m_parser->m_curr_event = Event::END;
        }
    }
    else {
        err_log.push_back(ErrorMessage("Unexpected value of m_curr_node: nullptr"));    // todo: err_message
        m_parser->m_curr_event = Event::END;
    }
}

void Skip_AddOp::apply() {
    m_parser->m_curr_name++;
    m_parser->m_curr_event = m_parser->to_event(*m_parser->m_curr_name);
    AddOp::apply();
}

void GoToParent::apply() {
    if (m_parser->m_curr_node) {
        Node* parent = m_parser->m_curr_node->m_parent;
        if (parent) {
            m_parser->m_curr_node = parent;

            m_parser->m_curr_name++;
            m_parser->m_curr_event = m_parser->to_event(*m_parser->m_curr_name);
        }
        else {
            err_log.push_back(ErrorMessage("Unexpected value of m_parent: nullptr"));    // todo: err_message
            m_parser->m_curr_event = Event::END;
        }
    }
    else {
        err_log.push_back(ErrorMessage("Unexpected value of m_curr_node: nullptr"));    // todo: err_message
        m_parser->m_curr_event = Event::END;
    }
}

void Err_UndefName::apply() {
    err_log.push_back(ErrorMessage("Unexpected name"));    // todo: err_message
    m_parser->m_curr_event = Event::END;
}