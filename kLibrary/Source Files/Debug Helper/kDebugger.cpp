#include <Precompile.h>
#include <Utility/Debug Helper/kDebugger.hpp>

namespace util::kDebugger
{
	void BreakPointHere() noexcept
	{
		__debugbreak();
	}

}