#include "pch.hpp"
#include "NotImplementedException.hpp"

namespace klib::kDebug::kExceptions
{
	NotImplementedException::NotImplementedException()
	{}

	NotImplementedException::NotImplementedException(const char* what)
		: exception(what)
	{ }

	NotImplementedException::~NotImplementedException() throw()
	{}

	char const* NotImplementedException::what() const
	{
		return exception::what();
	}
}