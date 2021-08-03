#pragma once
#include "AllocHeader.hpp"

namespace memory
{
	constexpr auto SignatureSize = sizeof(AllocHeader::Signature_Type);
	constexpr auto NodeSize = sizeof(AllocHeaderLinkedList::Node_t);
	constexpr auto ControlBlockSize = SignatureSize + NodeSize;
	
	constexpr auto MemoryLinkedListSize = sizeof(AllocHeaderLinkedList);
}
