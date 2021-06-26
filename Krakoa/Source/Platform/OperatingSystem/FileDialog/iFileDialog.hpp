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
		USE_RESULT virtual std::filesystem::path OpenFile(const FileDialogFilter& filter) const = 0;
		USE_RESULT virtual std::filesystem::path OpenFile(const wFileDialogFilter& filter) const = 0;
		USE_RESULT virtual std::filesystem::path SaveFile(const FileDialogFilter& filter) const = 0;
		USE_RESULT virtual std::filesystem::path SaveFile(const wFileDialogFilter& filter) const = 0;
	};
}
