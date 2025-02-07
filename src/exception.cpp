#include "exception.hpp"

InterbanqaException::InterbanqaException(const std::string& text)
{
	this->text = text;
}

const char* InterbanqaException::what() const noexcept
{
	return text.c_str();
}