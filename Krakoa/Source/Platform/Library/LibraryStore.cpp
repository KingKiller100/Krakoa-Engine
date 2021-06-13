#include "Precompile.hpp"
#include "LibraryStore.hpp"
#include "Win32/LibraryInstance_Win32.hpp"

#include "../../Logging/EngineLogger.hpp"
#include "../../Debug/Debug.hpp"
#include "../../Debug/Instrumentor.hpp"

#include <Utility/String/kToString.hpp>
#include <Utility/Platform/kPlatform.hpp>

namespace krakoa::library
{
	LibraryStore::LibraryStore()
	{
	}

	LibraryStore::~LibraryStore()
	{
		UnloadAll();
		libraries.clear();
	}

	void LibraryStore::Unload(const std::string_view& libName)
	{
		KRK_PROFILE_FUNCTION();
		if (Exists(libName))
		{
			const auto& lib = libraries.at(libName.data());
			KRK_INF(klib::ToString("Unloading library: {0}", libName));
			lib->Unload();
			KRK_INF(klib::ToString("Unloaded: {0}", lib->IsLoaded()));
		}
	}

	void LibraryStore::UnloadAll()
	{
		KRK_PROFILE_FUNCTION();
		for (auto&& libInfo : libraries)
		{
			const auto& name = libInfo.first;
			const auto& lib = libInfo.second;

			KRK_INF(klib::ToString("Unloading library: {0}", name));
			lib->Unload();
			KRK_INF(klib::ToString("Unloaded: {0}", lib->IsLoaded()));
		}
	}

	bool LibraryStore::Exists(const std::string_view& libName)
	{
		KRK_PROFILE_FUNCTION();
		return libraries.find(libName.data()) != libraries.end();
	}

	size_t LibraryStore::CountInstances() const
	{
		return libraries.size();
	}

	size_t LibraryStore::CountActiveInstances() const
	{
		size_t count = 0;
		for (auto&& libInfo : libraries)
		{
			if (const auto & lib = libInfo.second; lib->IsLoaded())
			{
				++count;
			}
		}
		return count;
	}

	bool LibraryStore::Load(const std::string_view& libName)
	{
		KRK_PROFILE_FUNCTION();
		if (Exists(libName))
		{
			return true;
		}

		KRK_INF(klib::ToString("Loading library: {0}", libName));

		const auto lib = CreateLibrary(libName);
		libraries.emplace(libName.data(), lib);
		const auto loaded = lib->IsLoaded();

		loaded ? KRK_DBG("Library loaded") : KRK_WRN("Library load failed");

		return loaded;
	}

	iLibraryInstance* LibraryStore::CreateLibrary(const std::string_view& libName) const
	{
		KRK_PROFILE_FUNCTION();
		switch (klib::GetPlatform())
		{
		case klib::PlatformOS::WINDOWS_32:
		case klib::PlatformOS::WINDOWS_64:
		case klib::PlatformOS::WINDOWS:
			return new LibraryInstance_Win32(libName.data());

		default:
			KRK_FATAL(klib::ToString("Attempting to load library on unsupported platform: {0}", libName));
			return nullptr;
		}
	}
}
