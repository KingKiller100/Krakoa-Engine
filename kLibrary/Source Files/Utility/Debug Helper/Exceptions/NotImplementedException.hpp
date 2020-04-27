#pragma once

#include <stdexcept>

namespace klib::kDebug::kExceptions
{
	class NotImplementedException : public std::runtime_error
	{
	public:
		NotImplementedException(const char* what);

		~NotImplementedException() throw();

		char const* what() const override;
	};
}