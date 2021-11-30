#ifndef __ERROR__
#define __ERROR__

#include <list>
#include <string>

class ErrorMessage
{
public:
    std::string m_descr;

    ErrorMessage(std::string err_descr) : m_descr(err_descr) {}
};

extern std::list<ErrorMessage> err_log;
#endif