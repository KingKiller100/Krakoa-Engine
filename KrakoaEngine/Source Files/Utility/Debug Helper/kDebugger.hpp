#pragma once
#include <Utility/File System/kFileSystem.hpp>

namespace util
{
	namespace kDebugger
	{
		template<class CharType>
		inline constexpr void CheckRemoteDebuggerAttached(const CharType* filename) noexcept
		{
#ifdef  _DEBUG
			if (::IsDebuggerPresent() == TRUE)
				return;

			while (kFileSystem::CheckFileExists(filename))
			{
				if (::IsDebuggerPresent() == TRUE)
				{
					break;
				}
			}
#endif
		}

		void BreakPointHere() noexcept;
	}
}

