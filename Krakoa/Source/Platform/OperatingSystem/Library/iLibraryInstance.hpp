#pragma once

namespace krakoa::os::library
{
	class iLibraryInstance
	{
	public:
		virtual ~iLibraryInstance() = default;

		virtual void Load(const char* filename) = 0;
		virtual void Unload() = 0;
		virtual bool IsLoaded() const = 0;
		virtual void* GetFunction(const char* funcName) = 0;
	};
}
