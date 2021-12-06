#include some_libs

using std::Node;
using std::Name;
using std::NameType;

void Evaluator::eval(Node* curr)
{
    // Creation of lists for results; let them have node* type,
    // because the returned type from eval may be s_expresssion 
    // and evel list with its parameters and command.
    // +1 for return
    list m_results = new list<Node*>;
    for (auto i = cur->m_chld.begin(); i != cur->m_chld.end(); i++)
    {
        m_results[i] = eval(i);
        if (m_results[i] == nullptr)
        {
            delete [] m_results;
            return nullptr;
        }
    }
    
    // Check: the first in list_chld must be operator
    // if (!findname(. ...  ))
    /* if (curr->m_name->m_type != OP)
    {
        delete [] m_results;
        // TODO: error message
        return nullptr;
    }*/ 
    
    // Do operation
    switch(m_results[0]->m_name) {
    case "+":
        bool flag = 0;
        for (auto i = cur->m_chld.begin(); i != cur->m_chld.end(); i++) if(i->m_name->m_type != NUM) flag = true;
        
        // Any child has not type number
        if (flag)
        {
            return nullptr;
            break;
        }
        
    
    
    
    
    // All childs were evaluated; time to eval function in the head
    // of cur to all its arguements
    
   
            
            
            
    
}
