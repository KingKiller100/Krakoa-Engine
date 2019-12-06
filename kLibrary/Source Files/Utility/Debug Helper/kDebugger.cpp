#include <pch.hpp>
#include <Utility/Debug Helper/kDebugger.hpp>

#include <Utility/File System/kFileSystem.hpp>

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