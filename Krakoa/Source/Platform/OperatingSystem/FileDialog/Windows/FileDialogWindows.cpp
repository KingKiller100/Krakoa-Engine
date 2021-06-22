#include "Precompile.hpp"
#include "FileDialogWindows.hpp"

#include "../../../../Core/Application.hpp"

#include <Windows.h>
#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

namespace krakoa::os
{
	FileDialogWindows::FileDialogWindows()
		= default;

	FileDialogWindows::~FileDialogWindows()
		= default;
	
	std::string FileDialogWindows::OpenFile(const std::string_view filter)
	{
		OPENFILENAMEA ofn;
		std::array<char, 260> file;
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = glfwGetWin32Window(GetWindow().GetNativeWindow<GLFWwindow>());
		ofn.lpstrFile = file.data();
		ofn.nMaxFile = file.size();
		ofn.lpstrFilter = filter.data();
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetOpenFileNameA(&ofn) == TRUE)
		{
			return ofn.lpstrFile;
		}
		return {};
	}

	std::string FileDialogWindows::SaveFile(const std::string_view filter)
	{
		OPENFILENAMEA ofn;
		std::array<char, 260> file;
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = glfwGetWin32Window(GetWindow().GetNativeWindow<GLFWwindow>());
		ofn.lpstrFile = file.data();
		ofn.nMaxFile = file.size();
		ofn.lpstrFilter = filter.data();
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetSaveFileNameA(&ofn) == TRUE)
		{
			return ofn.lpstrFile;
		}
		return {};
	}
}
