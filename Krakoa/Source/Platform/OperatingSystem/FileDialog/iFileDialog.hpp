#pragma once

#include <HelperMacros.hpp>
#include <filesystem>

#include "FileDialogFilter.hpp"

namespace krakoa::os
{
	class iFileDialog
	{
	public:
		virtual ~iFileDialog() = default;
		USE_RESULT virtual std::filesystem::path OpenFile(FileDialogFilter& filter) = 0;
		USE_RESULT virtual std::filesystem::path OpenFile(const std::wstring_view filter) = 0;
		USE_RESULT virtual std::filesystem::path SaveFile(const std::string_view filter) = 0;
		USE_RESULT virtual std::filesystem::path SaveFile(const std::wstring_view filter) = 0;
	};
}
