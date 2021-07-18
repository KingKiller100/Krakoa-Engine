#pragma once

#include "../Library/iLibraryInstance.hpp"
#include "../../Windows/IncludeWindowsLite.hpp"
#include <unordered_map>

#if defined(_WIN32) || defined(KRAKOA_OS_WINDOWS)

namespace krakoa::os::library
{
	class LibraryInstance_Windows final : public iLibraryInstance
	{
	public:
		LibraryInstance_Windows();
		LibraryInstance_Windows(const char* dllName);
		~LibraryInstance_Windows() override;
		void Load(const char* dllName) override;
		void Unload() override;
		bool IsLoaded() const override;
		void* GetFunction(const char* funcName) override;

	private:
		::HINSTANCE instance;
		std::unordered_map<std::string, FARPROC> functions;
	};
}

#endif
