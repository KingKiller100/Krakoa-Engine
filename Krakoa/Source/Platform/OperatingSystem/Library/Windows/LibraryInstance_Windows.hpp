#pragma once

#include "../iLibraryInstance.hpp"
#include "../../../Windows/IncludeWindowsLite.hpp"

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
		void LoadFunction(const char* funcName, void*& outFunc) override;

	private:
		::HINSTANCE instance;
	};
}
