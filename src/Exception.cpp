#include "Exception.hpp"
#include <iostream>

Exception::Exception(std::string e)
{
	mesg = e;
}

const char * Exception::what (  )
{
	return mesg.c_str();
}
