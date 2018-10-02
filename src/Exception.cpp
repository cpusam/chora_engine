#include "Exception.hpp"
#include <iostream>

Exception::Exception(const std::string e): std::runtime_error(e)
{
	mesg = e;
}

Exception::Exception(const char * e): std::runtime_error(e)
{
	mesg = e;
}

const char * Exception::what (  )
{
	return mesg.c_str();
}
