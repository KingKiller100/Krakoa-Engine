#pragma once
#include <string>

namespace krakoa::debug
{
	std::string UnwrapNestedExceptions(std::exception_ptr ePtr = std::current_exception());
}
