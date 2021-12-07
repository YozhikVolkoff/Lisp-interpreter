#include "evaluator.h"
#include <stdlib.h>


Node* Evaluator::eval(Node* curr)
{
    // Creation of lists for results; let them have node* type,
    // because the returned type from eval may be s_expresssion 
    // and evel list with its parameters and command.
    list<Node*> m_result;
    for (auto i = curr->m_chld.begin(); i != curr->m_chld.end(); ++i)
    {
        m_result.push_back(eval(*i));

        // If nullptr returned, the error occurs
        if (!m_result.back()) {
            m_result.clear();
            return nullptr;
        }
	
    }
    
    // Do operation
    if(curr->m_name->m_str == "+") {

	// Sum requires down to zero arguements
        bool flag = 0;
        for (auto i = m_result.begin(); i != m_result.end(); ++i) {
            if((*i)->m_name->m_type != NameType::NUM) flag = true;
        }
        if (flag) {
            m_result.clear();
            return nullptr;
        }
           
        Name* t_name = new Name();
        t_name->m_value = 0.0;
        t_name->m_type = NameType::NUM;
        Node* t = new Node(t_name);
        
        for (auto i = m_result.begin(); i != m_result.end(); ++i) {
            t->m_name->m_value += atof((*i)->m_name->m_str.c_str());
        }
        t->m_name->m_str = std::to_string(t->m_name->m_value);
        return t;
    }
    
    else if(curr->m_name->m_str == "-") {

	// Minus requires at least one arguement
	bool flag = 0;
        if (m_result.size() < 1) {
            flag = true;
            m_result.clear();
            return nullptr;
        }

        // Check for all NUMs
        Node* first = m_result.front();
        m_result.pop_front();
        if (first->m_name->m_type != NameType::NUM) flag = true;

        for (auto i = m_result.begin(); i != m_result.end(); ++i) {
            if ((*i)->m_name->m_type != NameType::NUM) flag = true;
        }
        if (flag) {
            m_result.clear();
            return nullptr;
        }

        Name* t_name = new Name();

	// If one argiement, is't just changing sign
	if (m_result.size() == 0) {
	    t_name->m_value = -1 * (atof(first->m_name->m_str.c_str()));
	}
	else {
	    t_name->m_value = atof(first->m_name->m_str.c_str());
        }
	t_name->m_type = NameType::NUM;
        Node* t = new Node(t_name);

        for (auto i = m_result.begin(); i != m_result.end(); ++i) {
            t->m_name->m_value -= atof((*i)->m_name->m_str.c_str());
        }
        t->m_name->m_str = std::to_string(t->m_name->m_value);

        return t;
    }
    else if(curr->m_name->m_str == "*") {

	// Muttiplation requires down to zero arguements
        bool flag = 0;
        for (auto i = m_result.begin(); i != m_result.end(); ++i) {
            if((*i)->m_name->m_type != NameType::NUM) flag = true;
        }
        if (flag) {
            m_result.clear();
            return nullptr;
        }

        Name* t_name = new Name();
        t_name->m_value = 1.0;
        t_name->m_type = NameType::NUM;
        Node* t = new Node(t_name);

        for (auto i = m_result.begin(); i != m_result.end(); ++i) {
            t->m_name->m_value *= atof((*i)->m_name->m_str.c_str());
        }
        t->m_name->m_str = std::to_string(t->m_name->m_value);

        return t;
    }

    else if(curr->m_name->m_str == "/") {

	// Division requires min 2 arguements
        bool flag = 0;
	if (m_result.size() < 2) {
	    flag = true;
	    m_result.clear();
	    return nullptr;
	}

	// Check for all NUMs and not zero except first arguement
	Node* first = m_result.front();
        m_result.pop_front();
	if (first->m_name->m_type != NameType::NUM) flag = true;

        for (auto i = m_result.begin(); i != m_result.end(); ++i) {
            if ((*i)->m_name->m_type != NameType::NUM) flag = true;
	    if (atof((*i)->m_name->m_str.c_str()) == 0) flag = true;
        }
        if (flag) {
            m_result.clear();
            return nullptr;
        }

        Name* t_name = new Name();
        t_name->m_value = atof(first->m_name->m_str.c_str());
        t_name->m_type = NameType::NUM;
        Node* t = new Node(t_name);
	
        for (auto i = m_result.begin(); i != m_result.end(); ++i) {
            t->m_name->m_value /= atof((*i)->m_name->m_str.c_str());
        }
        t->m_name->m_str = std::to_string(t->m_name->m_value);

        return t;
    }    
            
    else if(curr->m_name->m_str == "print")
    {
        
        // Simple print for just value in all nodes
	for (auto i = m_result.begin(); i != m_result.end(); ++i) {
            std::cout << atof((*i)->m_name->m_str.c_str()) << "\n" << std::endl;
        }
        return nullptr;  
    }
    else {
	Name* t_name = new Name();
        t_name->m_str = curr->m_name->m_str;
        t_name->m_type = NameType::NUM;
        Node* t = new Node(t_name);
	return t;
    }
}
