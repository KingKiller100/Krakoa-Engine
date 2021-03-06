﻿#pragma once
#include "AllocHeader.hpp"

namespace memory
{
	constexpr auto SignatureSize = sizeof(Signature_Type);
	constexpr auto AllocHeaderSize = sizeof(AllocHeader);
	constexpr auto ControlBlockSize = SignatureSize + AllocHeaderSize;

	template<typename T>
	constexpr auto MemoryLinkedListSize = sizeof(MemoryLinkedList<T>);
}
