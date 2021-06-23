#pragma once

#include "../iFileDialog.hpp"
#include <Utility/FileSystem/kPathString.hpp>
#include <Utility/String/kStringConverter.hpp>

#include <Windows.h>
#include <commdlg.h>

#include <functional>

namespace krakoa::os
{
	class FileDialogFilter;

	class FileDialogWindows : public iFileDialog
	{
	public:
		template<typename T>
		using PathString = klib::PathString<T, std::pmr::polymorphic_allocator<T>>;

		using OpenFileDialogFunc = std::function<BOOL(::OPENFILENAMEW*)>;
	
	public:
		FileDialogWindows();
		~FileDialogWindows() override;

		USE_RESULT std::filesystem::path OpenFile(FileDialogFilter& filter) override;
		USE_RESULT std::filesystem::path OpenFile(const std::wstring_view filter) override;
		USE_RESULT std::filesystem::path SaveFile(const std::string_view filter) override;
		USE_RESULT std::filesystem::path SaveFile(const std::wstring_view filter) override;
		
	private:
		PathString<wchar_t> directory;
	};
}
