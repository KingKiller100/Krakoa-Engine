#include "Precompile.hpp"
#include "MemoryOperators.hpp"
#include "MemoryUtil.hpp"

#include "MemoryStructures/Heap.hpp"
#include "MemoryStructures/MemoryTypeSizes.hpp"
#include "MemoryStructures/HeapFactory.hpp"

#include "MemoryDebug.hpp"

void* operator new []( const size_t bytes, memory::Heap* pHeap )
{
	return operator new( bytes, pHeap );
}

void* operator new( const size_t bytes )
{
	return operator new( bytes, memory::HeapFactory::GetDefaultHeap() );
}

void* operator new []( const size_t bytes )
{
	return operator new( bytes, memory::HeapFactory::GetDefaultHeap() );
}

void* operator new( const size_t bytes, memory::Heap* pHeap ) // Pads Control Blocks
{
#ifndef  KRAKOA_RELEASE
	MEM_ASSERT( bytes != 0 || bytes < std::numeric_limits<size_t>::max(),
		"Illegal amount of bytes requested" );

	if ( memory::Heap::s_TotalLifetimeBytesAllocated < std::numeric_limits<size_t>::max() )
		memory::Heap::s_TotalLifetimeBytesAllocated += bytes;

	const size_t requestedBytes = memory::NodeSize + bytes + memory::SignatureSize; // Alignment in memory
	auto* allocNode = memory::Allocate<memory::AllocHeaderNode>( requestedBytes );
	return memory::CreateNode( allocNode, bytes, pHeap ); // Returns pointer to the start of the object's data
#else
	return std::malloc(bytes);
#endif
}

void operator delete []( void* ptr )
{
	operator delete( ptr );
}

void operator delete( void* ptr )
{
	if ( ptr == nullptr ) return;

#ifndef KRAKOA_RELEASE

	auto* allocNode = memory::GetNodeFromDataPointer( ptr );
	memory::DestroyNode( allocNode );
	memory::Deallocate( allocNode );
#else
	std::free(ptr);
#endif
}
