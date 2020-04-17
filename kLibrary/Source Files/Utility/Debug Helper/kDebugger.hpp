#pragma once

namespace klib
{
	namespace kDebug
	{
		void CheckRemoteDebuggerAttached(const char* filename) noexcept;

		void BreakPointHere() noexcept;
	}
}

