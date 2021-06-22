#pragma once

#include <filesystem>

namespace krakoa::os
{
	class iFileDialog
	{
	public:
		virtual ~iFileDialog() = default;
		virtual std::string OpenFile(const std::string_view filter) = 0;
		virtual std::string SaveFile(const std::string_view filter) = 0;
	};
}
