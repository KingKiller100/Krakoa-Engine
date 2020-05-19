#pragma once

#include <stdexcept>

namespace klib::kDebug::errors
{
	class NotImplementedException final : public std::runtime_error
	{
	public:
		NotImplementedException(const char* what);

		~NotImplementedException() noexcept;

		char const* what() const override;
	};
}