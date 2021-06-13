#include "Precompile.hpp"
#include "LibraryInstance_Win32.hpp"

namespace krakoa::library
{
	LibraryInstance_Win32::LibraryInstance_Win32()
		: instance(nullptr)
	{
	}

	LibraryInstance_Win32::LibraryInstance_Win32(const char* dllName)
	{
		Load(dllName);
	}

	LibraryInstance_Win32::~LibraryInstance_Win32()
	{
		if (!IsLoaded())
		{
			Unload();
		}
	}

	void LibraryInstance_Win32::Load(const char* dllName)
	{
		instance = ::LoadLibraryA(dllName);
	}

	void LibraryInstance_Win32::Unload()
	{
		::FreeLibrary(instance);
	}

	bool LibraryInstance_Win32::IsLoaded() const
	{
		return instance != nullptr && instance != INVALID_HANDLE_VALUE;
	}

	void LibraryInstance_Win32::LoadFunction(const char* funcName, void* outFunc)
	{
		outFunc = ::GetProcAddress(instance, funcName);
	}
}
