#include "pch.hpp"
#include "NotImplementedException.hpp"

namespace klib::kDebug::kExceptions
{
	NotImplementedException::NotImplementedException(const char* what)
		: runtime_error(what)
	{ }

	NotImplementedException::~NotImplementedException() throw()
	{}

	char const* NotImplementedException::what() const
	{
		return runtime_error::what();
	}
}