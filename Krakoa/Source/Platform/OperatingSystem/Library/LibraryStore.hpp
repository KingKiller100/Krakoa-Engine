#pragma once
#include "iOSLibrary.hpp"
#include "iOSLibraryLoader.hpp"

#include "../../../Core/PointerTypes.hpp"
#include "../../../Logging/EngineLogger.hpp"

#include <string>
#include <functional>
#include <unordered_map>


namespace krakoa::os::library
{
	class LibraryStore
	{
	public:
		explicit LibraryStore(iOSLibraryLoader* libraryLoader);
		~LibraryStore();
		
		USE_RESULT Multi_Ptr<iOSLibrary> Request(const char* libName);
		
		void Unload( Multi_Ptr<iOSLibrary>& lib );
		void UnloadAll();
		USE_RESULT bool Exists(const std::string_view& libName);
		USE_RESULT size_t Size() const noexcept;
		USE_RESULT size_t Uses(const std::string_view& libName) const noexcept;
	
	private:
		bool Load(const std::string_view& libName);

	private:
		Solo_Ptr<iOSLibraryLoader> libLoader;
		std::unordered_map<std::string, Multi_Ptr<iOSLibrary>> libraries;
	};	
}
