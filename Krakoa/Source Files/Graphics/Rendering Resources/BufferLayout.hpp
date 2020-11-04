#pragma once

#include "../../Core/Logging/CoreLogger.hpp"


#include <string>
#include <vector>

namespace krakoa::graphics
{
	enum class ShaderDataType : uint8_t
	{
		NONE = 0, INT, INT2, INT3, INT4,
		FLOAT, FLOAT2, FLOAT3, FLOAT4,
		MAT2, MAT3, MAT4,
		BOOL
	};

	static uint32_t GetComponentCount(ShaderDataType type)
	{
		switch (type) {
		case krakoa::graphics::ShaderDataType::INT:	      return 1;
		case krakoa::graphics::ShaderDataType::INT2:      return 2;
		case krakoa::graphics::ShaderDataType::INT3:      return 3;
		case krakoa::graphics::ShaderDataType::INT4:      return 4;
		case krakoa::graphics::ShaderDataType::FLOAT:     return 1;
		case krakoa::graphics::ShaderDataType::FLOAT2:    return 2;
		case krakoa::graphics::ShaderDataType::FLOAT3:    return 3;
		case krakoa::graphics::ShaderDataType::FLOAT4:    return 4;
		case krakoa::graphics::ShaderDataType::MAT2:      return 2 * 2;
		case krakoa::graphics::ShaderDataType::MAT3:      return 3 * 3;
		case krakoa::graphics::ShaderDataType::MAT4:      return 4 * 4;
		case krakoa::graphics::ShaderDataType::BOOL:      return 1;
		default:
		{
			KRK_ASSERT(false, "Unknown ShaderDataType");
			return 0;
		}
		}
	}

	static uint32_t ShaderDataTypeBytes(ShaderDataType type)
	{
		uint32_t size = 0;

		switch (type) {
		case krakoa::graphics::ShaderDataType::INT:	      size = sizeof(int)  ; break;
		case krakoa::graphics::ShaderDataType::INT2:      size = sizeof(int)  ; break;
		case krakoa::graphics::ShaderDataType::INT3:      size = sizeof(int)  ; break;
		case krakoa::graphics::ShaderDataType::INT4:      size = sizeof(int)  ; break;
		case krakoa::graphics::ShaderDataType::FLOAT:     size = sizeof(float); break;
		case krakoa::graphics::ShaderDataType::FLOAT2:    size = sizeof(float); break;
		case krakoa::graphics::ShaderDataType::FLOAT3:    size = sizeof(float); break;
		case krakoa::graphics::ShaderDataType::FLOAT4:    size = sizeof(float); break;
		case krakoa::graphics::ShaderDataType::MAT2:      size = sizeof(float); break;
		case krakoa::graphics::ShaderDataType::MAT3:      size = sizeof(float); break;
		case krakoa::graphics::ShaderDataType::MAT4:      size = sizeof(float); break;
		case krakoa::graphics::ShaderDataType::BOOL:      size = sizeof(bool) ; break;
		default:
		{
			KRK_ASSERT(false, "Unknown ShaderDataType");
			return 0;
		}
		}

		return size * GetComponentCount(type);
	}

	struct BufferElement
	{
		std::string name;
		ShaderDataType type;
		uint32_t bytes;
		ptrdiff_t offset;
		bool normalized;

		BufferElement(ShaderDataType type, const std::string_view name, bool normalized = false)
			: name(name),
			type(type),
			bytes(ShaderDataTypeBytes(type)),
			offset(0),
			normalized(normalized)
		{}
	};

	class BufferLayout
	{
	public:
		BufferLayout() noexcept;
		BufferLayout(const std::initializer_list<BufferElement>& list);

		uint32_t GetStride() const noexcept;

		const std::vector<BufferElement>& GetElements() const noexcept;
		void CalculateOffsetAndStride() noexcept;

		USE_RESULT std::vector<BufferElement>::iterator begin();
		USE_RESULT std::vector<BufferElement>::iterator end();

		USE_RESULT std::vector<BufferElement>::const_iterator begin() const;
		USE_RESULT std::vector<BufferElement>::const_iterator end() const;

		size_t GetSize() const noexcept;

		const BufferElement& operator[](const size_t index) const;

	private:
		std::vector<BufferElement> elements;
		uint32_t stride;
	};
}