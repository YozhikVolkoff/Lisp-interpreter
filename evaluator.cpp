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

    string op = curr->m_name->m_str;

    if (op == "cond") {
	if (curr->m_chld.size() % 2 != 0 || curr->m_chld.size() < 2) {
	    err_log.push_back(ErrorMessage("cond: not enough arguments"));
	    return nullptr;
	}
	Value* v = new Value;
	
	for (auto i = curr->m_chld.begin(); i != curr->m_chld.end(); ++i) {
	    v = eval(*i);
	    if (!v) {
		err_log.push_back(ErrorMessage("cond: condition not evaluated"));
		delete v;
		return nullptr;
	    }
	    if (v->type != ValueType::BOOL) {
		err_log.push_back(ErrorMessage("cond: condition not BOOL"));
		delete v;
		return nullptr;
	    }
	    if (get<2>(v->val))
	    {
		++i;
		v = eval(*i);
		if (v) return v;
		else {
		    err_log.push_back(ErrorMessage("cond: form not evaluated"));
		    delete v;
		    return nullptr;
		}
	    }
	    else ++i;
	}
	
	*v = Value(false);
	return v;
    }
    

    list<Value*> m_result;
    for (auto i = curr->m_chld.begin(); i != curr->m_chld.end(); ++i) {
        m_result.push_back(eval(*i));

        // If nullptr returned, the error occurs
        if (!m_result.back()) {	
	    err_log.push_back(ErrorMessage("child not evaluated"));
	    for (auto i = m_result.begin(); i != m_result.end(); i++) delete *i;
            return nullptr;
        }
    }
    
    if (op == "+") {

	// Sum requires down to zero arguements
        bool flag = 0;
        for (auto i = m_result.begin(); i != m_result.end(); ++i) {
            if ((*i)->type != ValueType::NUM) flag = true;	
        }
        if (flag) {
            err_log.push_back(ErrorMessage("+: not a NUM"));
	    for (auto i = m_result.begin(); i != m_result.end(); i++) delete *i;
            return nullptr;
        }
           
	double r = 0.0;

        for (auto i = m_result.begin(); i != m_result.end(); ++i) {
	    r += get<0>((*i)->val);
        }

	Value* v = new Value(r);
	for (auto i = m_result.begin(); i != m_result.end(); i++) delete *i;
        return v;
    }
    
    else if(op == "-") {

	// Minus requires at least one arguement
	bool flag = 0;
        if (m_result.size() < 1) {
            err_log.push_back(ErrorMessage("-: not enough arguments"));
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
            err_log.push_back(ErrorMessage("-: not a NUM"));
	    for (auto i = m_result.begin(); i != m_result.end(); i++) delete *i;
            return nullptr;
        }

	double r = 0.0;

	// If one argument, is't just changing sign
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
	for (auto i = m_result.begin(); i != m_result.end(); i++) delete *i;
        return v;
    }
    else if(op == "*") {

	// Muttiplation requires down to zero arguements
        bool flag = 0;
        for (auto i = m_result.begin(); i != m_result.end(); ++i) {
            if ((*i)->type != ValueType::NUM) flag = true;
        }
        if (flag) {
            err_log.push_back(ErrorMessage("*: not a NUM"));
	    for (auto i = m_result.begin(); i != m_result.end(); i++) delete *i;
            return nullptr;
        }

        double r = 1.0;

        for (auto i = m_result.begin(); i != m_result.end(); ++i) {
            r *= get<0>((*i)->val);
        }

        Value* v = new Value(r);
	for (auto i = m_result.begin(); i != m_result.end(); i++) delete *i;
        return v;
    }

    else if(op == "/") {

	// Division requires min 1 arguement
        bool f_num = false;
	bool f_zero = false;
	if (m_result.size() < 1) {
	    err_log.push_back(ErrorMessage("/: not enough arguments"));
	    return nullptr;
	}

	// Check for all NUMs and not zero except first arguement
	Value* first = m_result.front();
        m_result.pop_front();
	if (first->type != ValueType::NUM) f_num = true;
	if (get<0>(first->val) == 0 and m_result.size() == 0) f_zero = true;

        for (auto i = m_result.begin(); i != m_result.end(); ++i) {
            if ((*i)->type != ValueType::NUM) f_num = true;
	    if (get<0>((*i)->val) == 0) f_zero = true;
        }
	
        if (f_num) {
            err_log.push_back(ErrorMessage("/: not a NUM"));
	    for (auto i = m_result.begin(); i != m_result.end(); i++) delete *i;
            return nullptr;
        }
	if (f_zero) {
	    err_log.push_back(ErrorMessage("/: division by zero"));
	    for (auto i = m_result.begin(); i != m_result.end(); i++) delete *i;
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
	for (auto i = m_result.begin(); i != m_result.end(); i++) delete *i;
        return v;
    } 

    else if (op == "=" || op == "/=") {
	
	// Comparison two NUMs of BOOLs or return t if zero arguements
	Value* v = new Value;

	if (m_result.size() == 0) {
	    *v = (op == "=") ? Value(true) : Value(false);
            return v;
        }
	else if (m_result.size() == 2) {
	    Value* f = m_result.front();
	    Value* s = m_result.back();
	    if (f->type == ValueType::NUM && s->type == ValueType::NUM) {
		if (op == "=") *v = (get<0>(f->val) != get<0>(s->val)) ? Value(false) : Value(true);
		else *v = (get<0>(f->val) != get<0>(s->val)) ? Value(true) : Value(false);
		for (auto i = m_result.begin(); i != m_result.end(); i++) delete *i;
		return v;
            }
	    else if (f->type == ValueType::BOOL && s->type == ValueType::BOOL) {
		if (op == "=") *v = (get<2>(f->val) != get<2>(s->val)) ? Value(false) : Value(true);
                else *v = (get<2>(f->val) != get<2>(s->val)) ? Value(true) : Value(false);
		for (auto i = m_result.begin(); i != m_result.end(); i++) delete *i;
                return v;
	    }
	    else {
		err_log.push_back(ErrorMessage("=/=: not NUMs or not BOOLs"));
	        for (auto i = m_result.begin(); i != m_result.end(); i++) delete *i;
                return nullptr;
	    }
	}
	else {
	    err_log.push_back(ErrorMessage("=/=: 2 or 0 arguments required"));
	    for (auto i = m_result.begin(); i != m_result.end(); i++) delete *i;
            return nullptr;
	}
    }

    else if (op == "<" || op == ">" || op == "<=" || op == ">=") {

        // Comparison two NUMs
        Value* v = new Value;

        if (m_result.size() == 2) {
            Value* f = m_result.front();
            Value* s = m_result.back();
            if (f->type == ValueType::NUM && s->type == ValueType::NUM) {
		if (op == "<")  *v = (get<0>(f->val) <  get<0>(s->val)) ? Value(true) : Value(false);
		if (op == ">")  *v = (get<0>(f->val) >  get<0>(s->val)) ? Value(true) : Value(false);
		if (op == ">=") *v = (get<0>(f->val) >= get<0>(s->val)) ? Value(true) : Value(false);
		if (op == "<=") *v = (get<0>(f->val) <= get<0>(s->val)) ? Value(true) : Value(false);
		for (auto i = m_result.begin(); i != m_result.end(); i++) delete *i;
		return v;
            }
	    else {
		err_log.push_back(ErrorMessage(">=<: not NUMs"));
	        for (auto i = m_result.begin(); i != m_result.end(); i++) delete *i;
                return nullptr;
	    }
        }
        else {
	    err_log.push_back(ErrorMessage(">=<: incorrect number of arguments"));
	    for (auto i = m_result.begin(); i != m_result.end(); i++) delete *i;
            return nullptr;
	}
    }

    else if (op == "and" || op == "or") {
	
	// AND operation for at least 2 BOOLs
	if (m_result.size() < 2) {
	    err_log.push_back(ErrorMessage("and/or: not enough arguments"));
	    for (auto i = m_result.begin(); i != m_result.end(); i++) delete *i;
            return nullptr;
	}
        
	bool flag = 0;
        for (auto i = m_result.begin(); i != m_result.end(); ++i) {
            if ((*i)->type != ValueType::BOOL) flag = true;
        }
        if (flag) {
	    err_log.push_back(ErrorMessage("and/or: not BOOLs"));
	    for (auto i = m_result.begin(); i != m_result.end(); i++) delete *i;
            return nullptr;
        }

	bool r = get<2>(m_result.front()->val);
	m_result.pop_front();
	
	if (op != "or") {
            for (auto i = m_result.begin(); i != m_result.end(); ++i) {
                r &= get<2>((*i)->val);
            }
	}
	else {
	    for (auto i = m_result.begin(); i != m_result.end(); ++i) {
                r |= get<2>((*i)->val);
            }
	}
	
        Value* v = new Value(r);
	for (auto i = m_result.begin(); i != m_result.end(); i++) delete *i;
        return v;
    }

    else if (op == "not") {
	
	// Switching one BOOL argument
	if (m_result.size() != 1) {
	    err_log.push_back(ErrorMessage("not: 1 argument required"));
	    for (auto i = m_result.begin(); i != m_result.end(); i++) delete *i;
            return nullptr;
	}

	if (m_result.front()->type != ValueType::BOOL) {
	    err_log.push_back(ErrorMessage("not: not BOOL argument"));
	    for (auto i = m_result.begin(); i != m_result.end(); i++) delete *i;
            return nullptr;
        }
	
	bool r = (! get<2>(m_result.front()->val));

	Value* v = new Value(r);
	for (auto i = m_result.begin(); i != m_result.end(); i++) delete *i;
	return v;
    }
    
    else if (op == "progn")
    {
	// simply run all childs and return the Value of last child;
	// if 0 arguments, returns nil
	if (m_result.size() == 0) {
	    Value* v = new Value(false);
	    return v;
	}

	Value* last = m_result.back();
	m_result.pop_back();
	Value* v;

	if (last->type == ValueType::NUM) {
	    v = new Value(get<0>(last->val));
	    for (auto i = m_result.begin(); i != m_result.end(); i++) delete *i;
	    delete last;
	    return v;
	}	
	else if (last->type == ValueType::BOOL) {
            v = new Value(get<2>(last->val));
	    for (auto i = m_result.begin(); i != m_result.end(); i++) delete *i;
	    delete last;
	    return v;
	}
	else if (last->type == ValueType::STR) {
            v = new Value(get<1>(last->val));
	    for (auto i = m_result.begin(); i != m_result.end(); i++) delete *i;
	    delete last;
	    return v;
	}
	else {
	    err_log.push_back(ErrorMessage("print: UNDEF argument"));
	    for (auto i = m_result.begin(); i != m_result.end(); i++) delete *i;
	    delete last;
	    return nullptr;
	}
    }
    
    else if(op == "print")
    {
        
        // Simple print for just one value
	if (m_result.size() != 1) {
	    err_log.push_back(ErrorMessage("print: 1 argument required"));
	    for (auto i = m_result.begin(); i != m_result.end(); i++) delete *i;
	    return nullptr;
	}	
	
	Value* first = m_result.front();
	m_result.pop_front();
	Value* v;

	if (first->type == ValueType::NUM) {
	    cout << "Out: " << get<0>(first->val) << endl;
	    v = new Value(get<0>(first->val));
	    delete first;
	    return v;
	}	
	else if (first->type == ValueType::BOOL) {
	    if (get<bool>(first->val)) cout << "Out: t" << endl;
            else cout << "Out: nil" << endl;
            v = new Value(get<2>(first->val));
	    delete first;
	    return v;
	}
	else if (first->type == ValueType::STR) {
            cout << "Out: " << get<1>(first->val) << endl;
            v = new Value(get<1>(first->val));
	    delete first;
	    return v;
	}
	else {
	    err_log.push_back(ErrorMessage("print: UNDEF argument"));
	    delete first;
	    return nullptr;
	}	
    }
    else if (op == "set") {
	
	// set requires two arguments, the first must be name
	if (m_result.size() != 2) {
	    err_log.push_back(ErrorMessage("set: 2 arguments required"));
	    m_result.clear();
	    return nullptr;
	}

	Name* to_be_set = new Name;
	to_be_set = find_name(curr->m_chld.front()->m_name->m_str, "name", 0);
	Value *to_set = new Value;
	to_set = m_result.back();
	Value *v = new Value;

	if (to_be_set) {
	    if (to_set->type == ValueType::NUM) to_be_set->m_value = Value(get<0>(to_set->val));
	    else if (to_set->type == ValueType::STR) to_be_set->m_value = Value(get<1>(to_set->val));
	    else if (to_set->type == ValueType::BOOL) to_be_set->m_value = Value(get<2>(to_set->val));
            *v = to_be_set->m_value;
	    return v;
	}
	
	err_log.push_back(ErrorMessage("set: UNDEF argument"));
	for (auto i = m_result.begin(); i != m_result.end(); i++) delete *i;
	return nullptr;
    }
    else {
	    
	Name* n = new Name;
	n = find_name(curr->m_name->m_str, "name", 0);
	Value* v = new Value;

	if (n) {	    
	    *v = n->get_value();
	    return v;
	}
	else {
	    if (curr->m_name->m_str == "t") *v = Value(true);
            else if (curr->m_name->m_str == "nil") *v = Value(false);
            else if (curr->m_name->m_type == NameType::NUM) *v = Value(stod(curr->m_name->m_str));
	    else if (curr->m_name->m_type == NameType::STR) *v = Value(curr->m_name->m_str);
   	    return v;
	}	
	return nullptr;
    }
    return nullptr;
}
