#pragma once
#include <string>

namespace krakoa::errors
{
	std::string UnwrapNestedExceptions(std::exception_ptr ePtr = std::current_exception());
}
