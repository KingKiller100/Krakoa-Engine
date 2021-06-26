#pragma once

#include "../FileDialog/iFileDialog.hpp"
#include <Utility/FileSystem/kPathString.hpp>
#include <Utility/String/kStringConverter.hpp>

#if defined(_WIN32) || defined(KRAKOA_OS_WINDOWS)

#include <Windows.h>
#include <commdlg.h>

#include <functional>

namespace krakoa::os
{
	class FileDialogWindows : public iFileDialog
	{
	public:
		template<typename T>
		using PathString = klib::PathString<T, std::pmr::polymorphic_allocator<T>>;

		using OpenFileDialogFunc = std::function<BOOL(::OPENFILENAMEW*)>;
	
	public:
		FileDialogWindows();
		~FileDialogWindows() override;

		USE_RESULT std::filesystem::path OpenFile(const FileDialogFilter& filter) const override;
		USE_RESULT std::filesystem::path OpenFile(const wFileDialogFilter& filter) const override;
		USE_RESULT std::filesystem::path SaveFile(const FileDialogFilter& filter) const override;
		USE_RESULT std::filesystem::path SaveFile(const wFileDialogFilter& filter) const override;
		
	private:
		Solo_Ptr<PathString<wchar_t>> directory;
	};
}

#endif
