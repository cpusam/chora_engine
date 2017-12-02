#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <string>
#include <exception>

class Exception: public std::exception
{
	public:
		Exception(std::string e);
		virtual ~Exception() throw (){}
		const char * what (  );
	protected:
		std::string mesg;
	private:
};

#endif // EXCEPTION_HPP
