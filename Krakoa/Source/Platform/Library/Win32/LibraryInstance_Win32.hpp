#pragma once

#include "../iLibraryInstance.hpp"
#include "../../Windows/IncludeWindowsLite.hpp"

namespace krakoa::library
{
	class LibraryInstance_Win32 final : public iLibraryInstance
	{
	public:
		LibraryInstance_Win32();
		LibraryInstance_Win32(const char* dllName);
		~LibraryInstance_Win32() override;
		void Load(const char* dllName) override;
		void Unload() override;
		bool IsLoaded() const override;
		void LoadFunction(const char* funcName, void* outFunc) override;

	private:
		::HINSTANCE instance;
	};
}
