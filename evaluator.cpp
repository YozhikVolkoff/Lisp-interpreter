


void Evaluator::eval(Node* curr)
{
    // Creation of lists for results; let them have node* type,
    // because the returned type from eval may be s_expresssion 
    // and evel list with its parameters and command.
    // +1 for return
    list<Node*> m_result;
    for (auto i = cur->m_chld.begin(); i != cur->m_chld.end(); ++i)
    {
        m_result.push_back(eval(i));
        
        // If nullptr returned, the error occurs
        if (!m_result.back()) {
            m_result.clear();
            return nullptr;
        }
    }
    
    // Do operation
    switch(curr->m_name->m_str) {
    case "+":
        bool flag = 0;
        for (auto i = m_result.begin(); i != m_result.end(); ++i) {
            if(i->m_name->m_type != NUM) flag = true;
        }
        if (flag) {
            m_result.clear();
            return nullptr;
        }
           
        // If all results are NUM, it's possible to sum them
        Node* t = new Node;
        t->m_name = new Name;
        t->m_name->m_value = 0.0;
        t->m_name->m_type = NameType::NUM;
        
        for (auto i = m_result.begin(); i != m_result.end(); ++i) {
            t->m_name->m_value += i->m_name->m_value;
        }
            
        return t;
    
    case "-":
        bool flag = 0;
        for (auto i = m_result.begin(); i != m_result.end(); ++i) {
            if(i->m_name->m_type != NUM) flag = true;
        }
        if (flag) {
            m_result.clear();
            return nullptr;
        }
           
        // If all results are NUM, it's possible to sum them
        Node* t = new Node;
        t->m_name = new Name;
        t->m_name->m_value = 0.0;
        t->m_name->m_type = NameType::NUM;
        
        for (auto i = m_result.begin(); i != m_result.end(); ++i) {
            t->m_name->m_value -= i->m_name->m_value;
        }
            
        return t;
    
    case "*":
        bool flag = 0;
        for (auto i = m_result.begin(); i != m_result.end(); ++i) {
            if(i->m_name->m_type != NUM) flag = true;
        }
        if (flag) {
            m_result.clear();
            return nullptr;
        }
           
        // If all results are NUM, it's possible to sum them
        Node* t = new Node;
        t->m_name = new Name;
        t->m_name->m_value = 0.0;
        t->m_name->m_type = NameType::NUM;
        
        for (auto i = m_result.begin(); i != m_result.end(); ++i) {
            t->m_name->m_value *= i->m_name->m_value;
        }
            
        return t;
    
    case "/":
        bool flag = 0;
        for (auto i = m_result.begin(); i != m_result.end(); ++i) {
            if(i->m_name->m_type != NUM) flag = true;
        }
        if (flag) {
            m_result.clear();
            return nullptr;
        }
           
        // If all results are NUM, it's possible to sum them
        Node* t = new Node;
        t->m_name = new Name;
        t->m_name->m_value = 0.0;
        t->m_name->m_type = NameType::NUM;
        
        for (auto i = m_result.begin(); i != m_result.end(); ++i) {
            t->m_name->m_value /= i->m_name->m_value;
        }
            
        return t;
            
   case "print":
        
        // Simple print for just value in the first child node
        cout << curr->m_name->m_value << '\n' << endl;
            
        return nullptr;
        
}
