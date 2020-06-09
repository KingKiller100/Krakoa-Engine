#include "Precompile.hpp"
#include "AllocHeader.hpp"

#include "MemoryTypes.hpp"

#include "../../Core/Logging/MemoryLogger.hpp"

#include <Maths/BytesTypes.hpp>
#include <Utility/Format/kFormatToString.hpp>

namespace memory
{
	size_t AllocHeader::GetMemoryBookmark() const noexcept
	{
		return bookmark;
	}

	bool AllocHeader::VerifyHeader(AllocHeader* pHeader, bool enableAssert)
	{
		using namespace klib::kFormat;
		
		if (pHeader->signature != KRK_MEMSYSTEM_START_SIG)
		{
			if (enableAssert)
			{
				MEM_FATAL(false, ToString("CORRUPTED HEAP - Incorrect signature"
					" on a heap - memory Address: {0}\n",
					pHeader));
			}
			return false;
		}

		auto* const pMemEnd = REINTERPRET(memory::AllocHeader::Signature_Type*,
			reinterpret_cast<kmaths::Byte_Type*>(pHeader) + AllocHeaderSize + pHeader->bytes);

		if (*pMemEnd != KRK_MEMSYSTEM_END_SIG)
		{
			if (enableAssert)
			{
				MEM_FATAL(false,
					ToString("CORRUPTED HEAP - Incorrect end marker on"
						" a heap - memory address: {0}\n",
						pHeader));
			}
			return false;
		}
		
		return true; // Both correct
	}

	AllocHeader* AllocHeader::GetHeaderFromPointer(void* pData)
	{
		auto* pHeader = reinterpret_cast<AllocHeader*>(
			static_cast<kmaths::Byte_Type*>(pData)
			- AllocHeaderSize);
		MEM_ASSERT(VerifyHeader(pHeader));
		return pHeader;
	}
}
