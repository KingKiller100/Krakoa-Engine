#include "Precompile.hpp"
#include "FileDialogWindows.hpp"

#include "../LogOS.hpp"
#include "../FileDialog/FileDialogFilter.hpp"

#include "../../../Core/Application.hpp"

#include <Utility/FileSystem/kFileSystem.hpp>

#if defined(_WIN32) || defined(KRAKOA_OS_WINDOWS)

#include <Windows.h>
#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <filesystem>


namespace krakoa::os
{
	namespace
	{
		template<typename OpenFileNameT,typename OpenFileDialogFunc, typename T>
		std::filesystem::path OpenDialog(const T* filter, std::function<OpenFileDialogFunc> dialogFunc, int flags, const klib::Path& directory)
		{
			OpenFileNameT ofn;
			std::array<T, MAX_PATH> file{};
			const auto dir = directory.string<T>();

			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = glfwGetWin32Window(GetWindow().GetNativeWindow<GLFWwindow>());
			ofn.lpstrFile = file.data();
			ofn.nMaxFile = static_cast<DWORD>(file.size());
			ofn.lpstrInitialDir = dir.data();
			ofn.lpstrFilter = filter;
			ofn.nFilterIndex = 1;
			ofn.Flags = flags;

			if (dialogFunc(&ofn) == TRUE)
			{
				return ofn.lpstrFile;
			}

			auto& eh = iOperatingSystem::Pointer()->GetErrorHandler();
			eh.CheckForNewError();

			if (const auto errCode = eh.GetCode(); errCode != 0)
			{
				const auto errMsg = std::string(util::Fmt("[{0}]: {1}", errCode, eh.GetText()));
				LogOSError(errMsg);
			}

			return {};
		}
	}

	FileDialogWindows::FileDialogWindows()
		: directory(Make_Solo<PathString<wchar_t>>(klib::GetCurrentWorkingDirectory<wchar_t>().data()))
	{}

	FileDialogWindows::~FileDialogWindows()
		= default;

	std::filesystem::path FileDialogWindows::OpenFile(const FileDialogFilter& filter) const
	{
		auto* data = filter.GetFilter();
		const auto path = OpenDialog<OPENFILENAMEA, decltype(GetOpenFileNameA)>(
			data, GetOpenFileNameA, 
			OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR, 
			directory->data());
		*directory = path.parent_path().c_str();
		return path;
	}

	std::filesystem::path FileDialogWindows::OpenFile(const wFileDialogFilter& filter) const
	{
		const auto path = OpenDialog<OPENFILENAMEW, decltype(GetOpenFileNameW)>(
				filter.GetFilter(), GetOpenFileNameW,
				OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR,
				directory->data());
		*directory = path.parent_path().c_str();
		return path;
	}

	std::filesystem::path FileDialogWindows::SaveFile(const FileDialogFilter& filter) const
	{
		const auto path = OpenDialog<OPENFILENAMEA, decltype(GetOpenFileNameA)>(
				filter.GetFilter(), GetSaveFileNameA,
				OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR,
				directory->data());
		*directory = path.parent_path().c_str();
		return path;
	}

	std::filesystem::path FileDialogWindows::SaveFile(const wFileDialogFilter& filter) const
	{
		const auto path = OpenDialog<OPENFILENAMEW, decltype(GetOpenFileNameW)>(
			filter.GetFilter(), GetSaveFileNameW,
			OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR,
			directory->data());
		*directory = path.parent_path().c_str();
		return path;
	}
}

#endif
