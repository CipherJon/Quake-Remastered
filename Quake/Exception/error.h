#pragma once

#include <exception>
#include <string>

class SystemException
	: public std::exception
{
public:
	// Constructor with a custom error message
	explicit SystemException(const std::string& message)
		: _message(message) {}

	// Override the what() method to return the custom error message
	const char* what() const noexcept override {
		return _message.c_str();
	}

	// Virtual destructor
	virtual ~SystemException() noexcept {}

private:
	std::string _message;
};