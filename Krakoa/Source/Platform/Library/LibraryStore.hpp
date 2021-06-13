#pragma once
#include "iLibraryInstance.hpp"
#include "../../Core/PointerTypes.hpp"
#include <string>
#include <unordered_map>
#include "../../Logging/EngineLogger.hpp"


namespace krakoa::library
{
	class LibraryStore
	{
	public:
		LibraryStore();
		~LibraryStore();

		template<typename FuncT>
		USE_RESULT FuncT LoadFunc(const char* libName, const char* funcName)
		{
			if (!Exists(libName))
			{
				if (!Load(libName))
					return nullptr;
			}
			
			FuncT function = nullptr;
			const auto& lib = libraries.at(std::string(libName));
			lib->LoadFunction(funcName, reinterpret_cast<void*>(function));

			if (function == nullptr)
			{
				KRK_ERR(klib::ToString("Unable to load function \"{0}\" from library \"{1}\"", funcName, libName));
				return nullptr;
			}
			
			return function;
		}
		
		void Unload(const std::string_view& libName);
		void UnloadAll();
		USE_RESULT bool Exists(const std::string_view& libName);

	private:
		bool Load(const std::string_view& libName);
		iLibraryInstance* CreateLibrary(const std::string_view& libName);
	
	private:
		std::unordered_map<std::string, Solo_Ptr<iLibraryInstance>> libraries;
	};	
}
