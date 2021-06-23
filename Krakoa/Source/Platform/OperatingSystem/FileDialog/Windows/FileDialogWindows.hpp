#pragma once

#include "../iFileDialog.hpp"

namespace krakoa::os
{
	class FileDialogWindows : public iFileDialog
	{
	public:
		FileDialogWindows();
		~FileDialogWindows() override;

		std::filesystem::path OpenFile(const std::string_view filter) override;
		std::filesystem::path SaveFile(const std::string_view filter) override;
	};
}
