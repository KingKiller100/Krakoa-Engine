#pragma once

#include "../../Logging/EngineLogger.hpp"

#include <string>
#include <vector>

namespace krakoa::gfx
{
	enum class ShaderDataType : uint8_t
	{
		NONE = 0, INT, INT2, INT3, INT4,
		FLOAT, FLOAT2, FLOAT3, FLOAT4,
		MAT2, MAT3, MAT4,
		BOOL
	};

	extern uint32_t GetComponentCount(ShaderDataType type);

	extern uint32_t ShaderDataTypeBytes(ShaderDataType type);

	struct BufferElement
	{
		std::string name;
		ShaderDataType type;
		uint32_t bytes;
		ptrdiff_t offset;
		bool normalized;

		BufferElement(ShaderDataType type, std::string_view name, bool normalized = false);
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