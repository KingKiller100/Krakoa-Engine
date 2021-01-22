#pragma once

#include "Resources/iVertexArray.hpp"

#include <Utility/Enum/kEnum.hpp>

#include <cstdint>

namespace krakoa::graphics
{
	class Colour;

	class iRendererAPI
	{
	public:
		ENUM_STRUCT(API, std::uint8_t,
			NONE = 0
			, OPENGL
		);

	public:
		virtual ~iRendererAPI() = default;

		virtual void Initialize() = 0;
		virtual void SetViewport(const int x, const int y, const int width, const int height) const noexcept = 0;
		virtual void Clear() = 0;
		virtual void DrawIndexed(const iVertexArray& vertexArray, uint32_t count = 0) = 0;
		virtual void SetClearColour(const Colour& colour) = 0;

		inline static API GetAPI() noexcept { return api; }

	protected:
		virtual void OutputRenderingArchitecture() = 0;
		
	protected:
		static API api;
	};
	
}

