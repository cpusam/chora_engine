#include "Exception.hpp"
#include <iostream>

Exception::Exception(std::string e)
{
	mesg = e;
}

const char * Exception::what (  )
{
	std::cerr<<mesg<<std::endl;
	return static_cast<const char *>(mesg.c_str());
}
