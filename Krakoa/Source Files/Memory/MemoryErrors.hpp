#pragma once

#include <exception>

namespace memory::debug
{
	class MemoryError : public std::exception
	{
	public:
		MemoryError()
			= default;

	protected:
		MemoryError(const char* const message)
			: exception(message)
		{}
	};

	class MemoryPoolError : public MemoryError
	{
	public:
		MemoryPoolError()
			: MemoryError("Error in memory pool")
		{}

		MemoryPoolError(const char* message)
			: MemoryError(message)
		{}
	};

	class MemoryFullError : public MemoryError
	{
	public:
		MemoryFullError()
			: MemoryError("Memory pool does not have enough space for this data")
		{}
	};

	class UnknownPointerError : public MemoryError
	{
	public:
		UnknownPointerError()
			: MemoryError("Pointer given not from our pool")
		{}
	};

}
