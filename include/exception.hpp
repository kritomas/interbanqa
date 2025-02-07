#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <exception>
#include <string>

class InterbanqaException : public std::exception
{
private:
	std::string text;

public:
	InterbanqaException(const std::string& text);

	const char* what() const noexcept override;
};

#endif