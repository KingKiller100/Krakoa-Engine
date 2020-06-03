#pragma once

#include <exception>

namespace memory::debug
{
	class MemmoryError : public std::exception
	{
	public:
		MemmoryError()
			= default;

	protected:
		MemmoryError(const char* const message)
			: exception(message)
		{}
	};

	class MemoryFullError : public MemmoryError
	{
	public:
		MemoryFullError()
			: MemmoryError("Memory pool does not have enough space for this data")
		{}
	};

	class UnknownPointerError : public MemmoryError
	{
	public:
		UnknownPointerError()
			: MemmoryError("Pointer given not from our pool")
		{}
	};

}
