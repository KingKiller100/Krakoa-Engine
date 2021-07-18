#include "Precompile.hpp"
#include "LibraryInstance_Windows.hpp"

#if defined(_WIN32) || defined(KRAKOA_OS_WINDOWS)

namespace krakoa::os::library
{
	LibraryInstance_Windows::LibraryInstance_Windows()
		: instance(nullptr)
	{}

	LibraryInstance_Windows::LibraryInstance_Windows(const char* dllName)
	{
		Load(dllName);
	}

	LibraryInstance_Windows::~LibraryInstance_Windows()
	{
		Unload();
	}

	void LibraryInstance_Windows::Load(const char* dllName)
	{
		instance = ::LoadLibraryA(dllName);
	}

	void LibraryInstance_Windows::Unload()
	{
		functions.clear();
		if (::FreeLibrary(instance) != FALSE)
		{
			instance = nullptr;
		}
	}

	bool LibraryInstance_Windows::IsLoaded() const
	{
		return instance != nullptr && instance != INVALID_HANDLE_VALUE;
	}

	void* LibraryInstance_Windows::GetFunction(const char* funcName)
	{
		if (auto iter = functions.find(funcName); iter != functions.end())
		{
			return iter->second;
		}
		
		const auto func = ::GetProcAddress(instance, funcName);
		functions.emplace(funcName, func);
		return func;
	}
}

#endif
