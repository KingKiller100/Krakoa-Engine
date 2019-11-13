#pragma once
#include "../File System/kFileSystem.h"

namespace util
{
	namespace kDebugger
	{

		template<class CharT>
		inline void CheckRemoteDebuggerAttached(const CharT* filename)
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

		void BreakPointHere();		
	}
}

