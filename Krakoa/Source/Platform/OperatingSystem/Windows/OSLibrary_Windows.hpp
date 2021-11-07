#pragma once

#include "../Library/iOSLibrary.hpp"
#include "../../Windows/IncludeWindowsLite.hpp"
#include <unordered_map>

#if defined(_WIN32) || defined(KRAKOA_OS_WINDOWS)

namespace krakoa::os::library
{
	class OSLibrary_Windows final : public iOSLibrary
	{
	public:
		OSLibrary_Windows(std::string_view name, HMODULE libModule);
		~OSLibrary_Windows() override;
		
		bool IsLoaded() const override;
		[[nodiscard]] std::string_view GetName() const override;
		[[nodiscard]] std::vector<std::string> GetImportsID() const override;

	protected:
		void* ImportImpl(const char* funcName) override;

		[[nodiscard]] ::HINSTANCE GetHandle() const noexcept;
	
	protected:
		friend class OSLibraryLoader_Windows;

	private:
		::HINSTANCE instance;
		std::string name;
		std::unordered_map<std::string, FARPROC> imports;
	};
}

#endif
