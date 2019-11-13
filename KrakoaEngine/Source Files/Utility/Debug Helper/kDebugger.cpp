#include "Precompile.h"
#include "kDebugger.h"

#include "../File System/kFileSystem.h"


namespace util::kDebugger
{
	using namespace kFileSystem;

	void BreakPointHere()
	{
		__debugbreak();
	}

}