#include "Precompile.hpp"
#include "LibraryStore.hpp"

#include "../../../Debug/Debug.hpp"
#include "../../../Debug/Instrumentor.hpp"

#include "../../../Util/Fmt.hpp"

namespace krakoa::os::library
{
	LibraryStore::LibraryStore(std::function<CreateLibraryInstanceFunc> createFunc)
		: createInstanceFunc(std::move(createFunc))
	{}

	LibraryStore::~LibraryStore()
	{
		Clear();
	}

	void LibraryStore::Clear()
	{
		libraries.clear();
	}

	void LibraryStore::Unload(const std::string_view& libName)
	{
		KRK_PROFILE_FUNCTION();
		const auto libInfoIter = libraries.find(libName.data());
		if (libInfoIter != libraries.end())
		{
			const auto& lib = libInfoIter->second;
			LogOS(util::Fmt("Unloading library: {0}", libName));
			lib->Unload();
			LogOS(util::Fmt("Unloaded: {0}", !lib->IsLoaded()));
		}
	}

	void LibraryStore::UnloadAll()
	{
		KRK_PROFILE_FUNCTION();
		for (auto&& libInfo : libraries)
		{
			const auto& name = libInfo.first;
			const auto& lib = libInfo.second;

			LogOS(util::Fmt("Unloading library: {0}", name));
			lib->Unload();
			LogOS(util::Fmt("Unloaded: {0}", !lib->IsLoaded()));
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
			if (const auto& lib = libInfo.second;
				lib->IsLoaded())
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

		LogOS(util::Fmt("Loading library: {0}", libName));

		const auto lib = CreateLibrary(libName);
		libraries.emplace(libName.data(), lib);
		const auto loaded = lib->IsLoaded();

		loaded ? KRK_DBG("Library loaded") : LogOSError("Library load failed");

		return loaded;
	}

	iLibraryInstance* LibraryStore::CreateLibrary(const std::string_view& libName) const
	{
		KRK_PROFILE_FUNCTION();
		if (createInstanceFunc == nullptr)
		{
			return nullptr;
		}

		return createInstanceFunc(libName.data());
	}
}
