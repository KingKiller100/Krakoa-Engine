﻿#pragma once
#include "iOSLibrary.hpp"
#include "iOSLibraryLoader.hpp"

#include "../../../Core/PointerTypes.hpp"
#include "../../../Logging/EngineLogger.hpp"

#include <string>
#include <unordered_map>


namespace krakoa::os::library
{
	class LibraryStore
	{
		struct Report
		{
			std::string name;
			std::vector<std::string> importIDs;
		};

	public:
		explicit LibraryStore( iOSLibraryLoader* libraryLoader );
		~LibraryStore();

		USE_RESULT Multi_Ptr<iOSLibrary> Request( const char* libName );

		void Unload( Multi_Ptr<iOSLibrary> lib );
		void UnloadAll();
		USE_RESULT bool Exists( std::string_view libName );
		USE_RESULT size_t Size() const noexcept;
		USE_RESULT size_t Uses( std::string_view libName ) const noexcept;
		USE_RESULT std::vector<Report> GetReports() const;

	private:
		bool Load( std::string_view libName );

	private:
		Solo_Ptr<iOSLibraryLoader> libLoader;
		std::unordered_map<std::string, Multi_Ptr<iOSLibrary>> libraries;
	};
}
