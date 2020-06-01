#pragma once
#include "AllocHeader.hpp"

namespace memory
{
	constexpr auto SignatureBytes = sizeof(AllocHeader::Signature_Type);
	constexpr auto AllocHeaderBytes = sizeof(AllocHeader);
	constexpr auto MemoryControlBlockBytes = SignatureBytes + AllocHeaderBytes;
}
