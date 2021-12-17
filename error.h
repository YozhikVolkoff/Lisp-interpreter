//-----------------------------------------------------------------------------
//  Author      : Sheptunov Dmitry
//  Description : err_log - contains all error messages
//-----------------------------------------------------------------------------


#ifndef __ERROR__
#define __ERROR__

#include <list>
#include <string>

class ErrorMessage
{
public:
    std::string m_descr;

    ErrorMessage(std::string err_descr) : m_descr("Error! " + err_descr + "\n") {}
};

extern std::list<ErrorMessage> err_log;

#endif