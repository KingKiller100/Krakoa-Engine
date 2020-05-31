#pragma once
#include "AllocHeader.hpp"

namespace memory
{
	using Byte_Type = std::int8_t;
	using Byte_Ptr_Type = Byte_Type * ;
	
	constexpr auto SignatureBytes = sizeof(AllocHeader::Signature_Type);
	constexpr auto AllocHeaderBytes = sizeof(AllocHeader);
	constexpr auto MemoryControlBlockBytes = SignatureBytes + AllocHeaderBytes;
}
