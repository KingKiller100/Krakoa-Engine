#pragma once

#include "kDebugger.h"

namespace util
{
	
#if _DEBUG

	
	inline void __cdecl Assert(wchar_t const* txt, wchar_t const* file, wchar_t const* line)
	{

		kDebugger::BreakPointHere();
	}
	
#define kAssert(condition)\
	(void)\
	(\
		(condition == true) ||\
		Assert(_CRT_WIDE(#condition), _CRT_WIDE(__FILE__), _CRT_WIDE(__LINE__)\
	)\

#else
#define kAssert ((void)0)

#endif



}