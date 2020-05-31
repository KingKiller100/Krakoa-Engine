#include "Precompile.hpp"
#include "AllocHeader.hpp"

#include "HeapBase.hpp"
#include "MemoryTypes.hpp"

#include "../../Core/Logging/MemoryLogger.hpp"

#include <Utility/Format/kFormatToString.hpp>

namespace memory
{
	void AllocHeader::Verify(AllocHeader* pHeader)
	{
		using namespace klib::kFormat;
		
		MEM_FATAL(pHeader->signature == KRK_MEMSYSTEM_START_SIG,
			ToString("CORRUPTED HEAP - Incorrect signature"
				" on heap: \"{0}\" Address: {1}\n",
				pHeader->pHeap->GetName(),
				pHeader));
		
		auto* pBlockStart = reinterpret_cast<std::int8_t*>(pHeader);

		auto * pMemEnd = REINTERPRET(memory::AllocHeader::Signature_Ptr_Type,
			pBlockStart + AllocHeaderBytes + pHeader->bytes);
		
		MEM_FATAL(*pMemEnd == KRK_MEMSYSTEM_END_SIG,
			ToString("CORRUPTED HEAP - Incorrect end marker on"
				" heap: \"{0}\" Address: {1}\n",
				pHeader->pHeap->GetName(),
				pHeader));
	}

	AllocHeader* AllocHeader::GetHeaderFromPointer(void* pData)
	{
		auto* pHeader = reinterpret_cast<AllocHeader*>(
			static_cast<Byte_Ptr_Type>(pData)
			- AllocHeaderBytes);
		Verify(pHeader);
		return pHeader;
	}
}
