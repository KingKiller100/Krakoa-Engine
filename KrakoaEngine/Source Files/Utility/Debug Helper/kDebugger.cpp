#include "kDebugger.h"

#include "../File System/kFileSystem.h"

#include <Windows.h>

namespace util::kDebugger
{
	using namespace kFileSystem;
	
	void CheckRemoteDebuggerAttached(const char* filename)
	{
#ifdef  _DEBUG
		if (::IsDebuggerPresent() == TRUE)
			return;

		while (CheckFileExists(filename))
		{
			if (::IsDebuggerPresent() == TRUE)
			{
				break;
			}
		}
#endif
	}

	void BreakPointHere()
	{
		__debugbreak();
	}

}