#include "kDebugger.h"

#include <Windows.h>

namespace krakoa::kDebugger
{
	void CheckRemoteDebuggerAttached(const char* filename)
	{
#ifdef  _DEBUG
		const auto debuggerAttached = ::IsDebuggerPresent() == TRUE;
		
#endif
	}

	void BreakPointHere()
	{
		__debugbreak();
	}

}