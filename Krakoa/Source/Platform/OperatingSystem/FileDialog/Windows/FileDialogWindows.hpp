#pragma once

#include "../iFileDialog.hpp"

namespace krakoa::os
{
	class FileDialogWindows : public iFileDialog
	{
	public:
		FileDialogWindows();
		~FileDialogWindows() override;

		std::string OpenFile(const std::string_view filter) override;
		std::string SaveFile(const std::string_view filter) override;
	};
}
