#include some_libs

using std::Node;
using std::Name;

void Evaluator::eval(Node* cur)
{
    Name name = cut->Nam->m_string;
    for (auto i = cur->m_child.begin(); i != cur->m_child.end(); i++)
    {
        eval(i);
    }
    
}
