#pragma once

#include <exception>

namespace klib::exceptions
{
	class NotImplementedException : public std::exception
	{
		NotImplementedException();
		NotImplementedException(const char* what);

		~NotImplementedException() throw();

		char const* what() const override;
	};
}