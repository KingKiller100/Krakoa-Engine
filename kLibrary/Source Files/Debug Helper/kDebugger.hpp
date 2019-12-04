#pragma once

#include <HelperMacros.hpp>

#include <File System/kFileSystem.hpp>

namespace util
{
	namespace kDebugger
	{
		template<class CharType>
		inline constexpr void CheckRemoteDebuggerAttached(const CharType* filename) noexcept
		{
#ifdef  _DEBUG
			if (::IsDebuggerPresent() == IS_TRUE) // 
				return;

			while (kFileSystem::CheckFileExists(filename))
			{
				if (::IsDebuggerPresent() == IS_TRUE)
				{
					break;
				}
			}
#endif
		}

		void BreakPointHere() noexcept;
	}
}

