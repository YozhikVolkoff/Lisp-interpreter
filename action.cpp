#include "action.h"

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
        if (m_lexer->find_name(buffer, "kword", 0) == -1)
            m_lexer->find_name(buffer, "name", 1);
    
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