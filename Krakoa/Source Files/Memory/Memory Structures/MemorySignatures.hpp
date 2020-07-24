#pragma once

#include <cstdint>

// 3735929054
#define KRK_MEMSYSTEM_START_SIG 0xdeadc0de 

// 3735928559
#define KRK_MEMSYSTEM_END_SIG 0xdeadbeef

namespace memory
{
	using Signature_Type = std::uint32_t;
}