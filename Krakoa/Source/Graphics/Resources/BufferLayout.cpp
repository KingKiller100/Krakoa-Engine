#include "Precompile.hpp"
#include "BufferLayout.hpp"
#include "../../Debug/Debug.hpp"

namespace krakoa::gfx
{
	uint32_t GetComponentCount(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::INT: return 1;
		case ShaderDataType::INT2: return 2;
		case ShaderDataType::INT3: return 3;
		case ShaderDataType::INT4: return 4;
		case ShaderDataType::FLOAT: return 1;
		case ShaderDataType::FLOAT2: return 2;
		case ShaderDataType::FLOAT3: return 3;
		case ShaderDataType::FLOAT4: return 4;
		case ShaderDataType::MAT2: return 2 * 2;
		case ShaderDataType::MAT3: return 3 * 3;
		case ShaderDataType::MAT4: return 4 * 4;
		case ShaderDataType::BOOL: return 1;
		default:
			{
				KRK_FATAL("Unknown ShaderDataType");
				return 0;
			}
		}
	}

	uint32_t ShaderDataTypeBytes(ShaderDataType type)
	{
		uint32_t size = 0;

		switch (type)
		{
		case ShaderDataType::INT:
		case ShaderDataType::INT2:
		case ShaderDataType::INT3:
		case ShaderDataType::INT4: size = sizeof(int);
			break;
		case ShaderDataType::FLOAT:
		case ShaderDataType::FLOAT2:
		case ShaderDataType::FLOAT3:
		case ShaderDataType::FLOAT4: size = sizeof(float);
			break;
		case ShaderDataType::MAT2:
		case ShaderDataType::MAT3:
		case ShaderDataType::MAT4: size = sizeof(float);
			break;
		case ShaderDataType::BOOL: size = sizeof(bool);
			break;
		case ShaderDataType::NONE:
		default:
			{
				KRK_FATAL("Unknown ShaderDataType");
				return 0;
			}
		}

		return size * GetComponentCount(type);
	}

	BufferElement::BufferElement(ShaderDataType type, std::string_view name, bool normalized): name(name),
		type(type),
		bytes(ShaderDataTypeBytes(type)),
		offset(0),
		normalized(normalized)
	{}

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