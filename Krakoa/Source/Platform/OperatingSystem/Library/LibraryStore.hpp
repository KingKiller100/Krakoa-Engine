#pragma once
#include "iOSLibrary.hpp"

#include "../LogOS.hpp"
#include "../../../Core/PointerTypes.hpp"
#include "../../../Logging/EngineLogger.hpp"

#include <string>
#include <functional>
#include <unordered_map>


namespace krakoa::os::library
{
	using CreateLibraryInstanceFunc = iOSLibrary * (const char*);
	
	class LibraryStore
	{
	public:
		LibraryStore(std::function<CreateLibraryInstanceFunc> createFunc);
		~LibraryStore();
		
		USE_RESULT Weak_Ptr<iOSLibrary> LoadFunc(const char* libName);
		
		void Unload(const std::string_view& libName);
		void UnloadAll();
		void Clear();
		USE_RESULT bool Exists(const std::string_view& libName);
		USE_RESULT size_t CountInstances() const;
		USE_RESULT size_t CountActiveInstances() const;
	
	private:
		bool Load(const std::string_view& libName);
		USE_RESULT iOSLibrary* CreateLibrary(const std::string_view& libName) const;

	
	private:
		std::unordered_map<std::string, Multi_Ptr<iOSLibrary>> libraries;
		std::function<CreateLibraryInstanceFunc> createInstanceFunc;
	};	
}
