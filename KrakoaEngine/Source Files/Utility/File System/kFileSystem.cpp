#include "kFileSystem.h"

#include <cstdio>
#include <memory>

namespace krakoa::kFileSystem
{
	bool krakoa::kFileSystem::CheckFileExists(const char* filename)
	{
		const std::unique_ptr<FILE> file;
		auto filePtr = file.get();
		fopen_s(&filePtr, filename, "r");
		
		if (filePtr)
		{
			fclose(filePtr);
			return true;
		}
		return false;
	}
}

