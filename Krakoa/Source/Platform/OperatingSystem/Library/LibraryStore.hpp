#pragma once
#include "iLibraryInstance.hpp"
#include "../../../Core/PointerTypes.hpp"
#include "../../../Logging/EngineLogger.hpp"
#include <string>
#include <functional>
#include <unordered_map>


namespace krakoa::os::library
{
	using CreateLibraryInstanceFunc = iLibraryInstance * (const char*);
	
	class LibraryStore
	{
	public:
		LibraryStore(std::function<CreateLibraryInstanceFunc> createFunc);
		~LibraryStore();

		template<typename FuncT>
		USE_RESULT std::function<FuncT> LoadFunc(const char* libName, const char* funcName)
		{
			if (!Exists(libName))
			{
				if (!Load(libName))
					return nullptr;
			}
			
			FuncT* function = nullptr;
			const auto& lib = libraries.at(std::string(libName));
			lib->LoadFunction(funcName, (void*&)function);

			if (function == nullptr)
			{
				KRK_ERR(klib::ToString("Unable to load function \"{0}\" from library \"{1}\"", funcName, libName));
			}
			
			return function;
		}
		
		void Unload(const std::string_view& libName);
		void UnloadAll();
		void Clear();
		USE_RESULT bool Exists(const std::string_view& libName);
		USE_RESULT size_t CountInstances() const;
		USE_RESULT size_t CountActiveInstances() const;
	
	private:
		bool Load(const std::string_view& libName);
		USE_RESULT iLibraryInstance* CreateLibrary(const std::string_view& libName) const;
	
	private:
		std::unordered_map<std::string, Solo_Ptr<iLibraryInstance>> libraries;
		std::function<CreateLibraryInstanceFunc> createInstanceFunc;
	};	
}
