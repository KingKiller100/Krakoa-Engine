#include "Precompile.hpp"
#include "DefaultHeap.hpp"

//#include "MemoryTypes.hpp"
//#include <Utility/Format/kFormatToString.hpp>
//#include <Utility/Debug Helper/Exceptions/NotImplementedException.hpp>


namespace memory
{
	DefaultHeap::DefaultHeap() noexcept
		: HeapBase("Default")
	{}

	DefaultHeap::~DefaultHeap() noexcept
		= default;
}
