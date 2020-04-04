#include "pch.hpp"
#include "kDebugger.hpp"

#include "../File System/kFileSystem.hpp"

namespace klib::kDebugger
{
	void CheckRemoteDebuggerAttached(const char* filename) noexcept
	{
#ifdef  _DEBUG
		if (::IsDebuggerPresent() == IS_TRUE)
			return;

		while (kFileSystem::CheckFileExists(filename))
		{
			if (::IsDebuggerPresent() == IS_TRUE)
			{
				return;
			}
		}
#endif
	}

	void BreakPointHere() noexcept
	{
		__debugbreak();
	}

}