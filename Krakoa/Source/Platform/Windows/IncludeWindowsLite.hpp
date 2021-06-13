#pragma once

#if defined(KRAKOA_OS_WINDOWS) || defined(_WIN32)
#	define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#		include <Windows.h>
#	undef WIN32_LEAN_AND_MEAN
#endif
