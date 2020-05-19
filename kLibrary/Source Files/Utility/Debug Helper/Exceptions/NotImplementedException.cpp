#include "pch.hpp"
#include "NotImplementedException.hpp"

namespace klib::kDebug::errors
{
	NotImplementedException::NotImplementedException(const char* what)
		: runtime_error(what)
	{ }

	NotImplementedException::~NotImplementedException() noexcept
		= default;
}
