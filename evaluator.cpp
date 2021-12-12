#include "evaluator.h"
#include <stdlib.h>

using namespace::std;


Value* Evaluator::eval(Node* curr)
{
    /* 
    Creation of lists for results; let them have Value* type,
    because the returned type from eval may be s_expresssion 
    and evel list with its parameters and command.
    */    

    list<Value*> m_result;
    for (auto i = curr->m_chld.begin(); i != curr->m_chld.end(); ++i)
    {
        m_result.push_back(eval(*i));

        // If nullptr returned, the error occurs
        if (!m_result.back()) {
            m_result.clear();
            return nullptr;
        }
    }
    
    if(curr->m_name->m_str == "+") {

	// Sum requires down to zero arguements
        bool flag = 0;
        for (auto i = m_result.begin(); i != m_result.end(); ++i) {
            if ((*i)->type != ValueType::NUM) flag = true;	
        }
        if (flag) {
            m_result.clear();
            return nullptr;
        }
           
	double r = 0.0;

        for (auto i = m_result.begin(); i != m_result.end(); ++i) {
	    r += get<0>((*i)->val);
        }

	Value* v = new Value(r);
        return v;
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
        Value* first = m_result.front();
        m_result.pop_front();
        if (first->type != ValueType::NUM) flag = true;

        for (auto i = m_result.begin(); i != m_result.end(); ++i) {
            if ((*i)->type != ValueType::NUM) flag = true;
        }
        if (flag) {
            m_result.clear();
            return nullptr;
        }

	double r = 0.0;

	// If one argiement, is't just changing sign
	if (m_result.size() == 0) {
	    r = -1 * get<0>(first->val);
	}
	else {
	    r = get<0>(first->val);
        }

        for (auto i = m_result.begin(); i != m_result.end(); ++i) {
            r -= get<0>((*i)->val);
        }

	Value* v = new Value(r);
        return v;
    }
    else if(curr->m_name->m_str == "*") {

	// Muttiplation requires down to zero arguements
        bool flag = 0;
        for (auto i = m_result.begin(); i != m_result.end(); ++i) {
            if ((*i)->type != ValueType::NUM) flag = true;
        }
        if (flag) {
            m_result.clear();
            return nullptr;
        }

        double r = 1.0;

        for (auto i = m_result.begin(); i != m_result.end(); ++i) {
            r *= get<0>((*i)->val);
        }

        Value* v = new Value(r);
        return v;
    }

    else if(curr->m_name->m_str == "/") {

	// Division requires min 1 arguement
        bool flag = 0;
	if (m_result.size() < 1) {
	    flag = true;
	    m_result.clear();
	    return nullptr;
	}

	// Check for all NUMs and not zero except first arguement
	Value* first = m_result.front();
        m_result.pop_front();
	if (first->type != ValueType::NUM) flag = true;
	if (get<0>(first->val) == 0 and m_result.size() == 0) flag = true;

        for (auto i = m_result.begin(); i != m_result.end(); ++i) {
            if ((*i)->type != ValueType::NUM) flag = true;
	    if (get<0>((*i)->val) == 0) flag = true;
        }
	
        if (flag) {
            m_result.clear();
            return nullptr;
        }

	double r = 1.0;
	
	// If 1 arguement, it's just inverting against 1
	if (m_result.size() == 0) {
            r = 1 / get<0>(first->val);
        }
        else {
            r = get<0>(first->val);
        }

        for (auto i = m_result.begin(); i != m_result.end(); ++i) {
            r /= get<0>((*i)->val);
        }

	Value* v = new Value(r);
        return v;
    } 
            
    else if(curr->m_name->m_str == "print")
    {
        
        // Simple print for just one value
	if (m_result.size() != 1)
	{
	    m_result.clear();
	    return nullptr;
	}	

	Value* first = m_result.front();
	m_result.pop_front();
	Value* v;

	if (first->type == ValueType::NUM) {
	    cout << get<0>(first->val) << endl;
	    v = new Value(get<0>(first->val));
	    cout << "DOUBLE" << endl;
	}	
	else if (first->type == ValueType::BOOL) {
            cout << get<bool>(first->val) << endl;
            v = new Value(get<2>(first->val));
 	    cout << "BOOL" << endl;
	}
	else if (first->type == ValueType::STR) {
            cout << get<1>(first->val) << endl;
            v = new Value(get<1>(first->val));
	}
	else return nullptr;

        return v;
    }
    else {
	if (curr->m_name->m_str != "t" && curr->m_name->m_str != "nil") {
	    Value *v = new Value(stod(curr->m_name->m_str));
	    return v;
	}
	/* else {
	    Value *v;
	    if (curr->m_name->m_str == "t") v = new Value(true);
            else v = new Value(false);
            return v;
	} */
    }
}
