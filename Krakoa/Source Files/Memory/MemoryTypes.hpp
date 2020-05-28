#pragma once
#include "AllocHeader.hpp"

namespace memory
{
	using Byte_Type = char;
	using Byte_Ptr_Type = Byte_Type * ;
	
	constexpr auto signatureBytes = sizeof(decltype(AllocHeader::signature));
	constexpr auto allocHeaderBytes = sizeof(AllocHeader);
}
