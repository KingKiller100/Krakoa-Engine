#include "Precompile.hpp"
#include "BufferLayout.hpp"

namespace krakoa::graphics
{
	BufferLayout::BufferLayout() noexcept
		= default;

	BufferLayout::BufferLayout(const std::initializer_list<BufferElement>& list)
		: elements(list)
	{
		CalculateOffsetAndStride();
	}

	uint32_t BufferLayout::GetStride() const noexcept
	{
		return stride;
	}

	const std::vector<BufferElement>& BufferLayout::GetElements() const noexcept
	{
		return elements;
	}

	void BufferLayout::CalculateOffsetAndStride() noexcept
	{
		stride = 0;
		for (auto& element : elements)
		{
			element.offset = stride;
			stride += element.bytes;
		}
	}

	std::vector<BufferElement>::iterator BufferLayout::begin()
	{
		return elements.begin();
	}

	std::vector<BufferElement>::iterator BufferLayout::end()
	{
		return elements.end();
	}

	std::vector<BufferElement>::const_iterator BufferLayout::begin() const
	{
		return elements.cbegin();
	}

	std::vector<BufferElement>::const_iterator BufferLayout::end() const
	{
		return elements.cend();
	}

	size_t BufferLayout::GetSize() const noexcept
	{
		return elements.size();
	}

	const BufferElement & BufferLayout::operator[](const size_t index) const
	{
		return elements[index];
	}
}