#include "Precompile.hpp"
#include "AllocHeader.hpp"

#include "HeapBase.hpp"
#include "MemoryTypes.hpp"

#include "../../Core/Logging/MemoryLogger.hpp"

#include <Maths/BytesTypes.hpp>
#include <Utility/Format/kFormatToString.hpp>



namespace memory
{
	bool AllocHeader::VerifyHeader(AllocHeader* pHeader, bool enableAssert)
	{
		using namespace klib::kFormat;

		const auto correctStart = pHeader->signature == KRK_MEMSYSTEM_START_SIG;

		auto* pMemEnd = REINTERPRET(memory::AllocHeader::Signature_Ptr_Type,
			reinterpret_cast<kmaths::Byte_Type*>(pHeader) + AllocHeaderBytes + pHeader->bytes);

		const auto correctEnd = *pMemEnd == KRK_MEMSYSTEM_END_SIG;

		if (enableAssert)
		{
			MEM_FATAL(correctStart, ToString("CORRUPTED HEAP - Incorrect signature"
				" on a heap - memory Address: {0}\n",
				pHeader));

			MEM_FATAL(correctEnd,
				ToString("CORRUPTED HEAP - Incorrect end marker on"
					" a heap - memory address: {0}\n",
					pHeader));
		}

		return (correctStart & correctEnd); // Both correct
	}

	AllocHeader* AllocHeader::GetHeaderFromPointer(void* pData)
	{
		auto* pHeader = reinterpret_cast<AllocHeader*>(
			static_cast<kmaths::Byte_Type*>(pData)
			- AllocHeaderBytes);
		MEM_ASSERT(VerifyHeader(pHeader));
		return pHeader;
	}
}
