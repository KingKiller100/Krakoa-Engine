#pragma once

namespace klib
{
	namespace kDebugger
	{
		void CheckRemoteDebuggerAttached(const char* filename) noexcept;

		void BreakPointHere() noexcept;
	}
}

