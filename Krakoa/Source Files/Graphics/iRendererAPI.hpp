#pragma once

#include "Rendering Resources/iVertexArray.hpp"

namespace krakoa::graphics
{
	class Colour;

	class iRendererAPI
	{
	public:
		enum class ApiType : uint8_t
	{
		NONE = 0,
		OPENGL,
	};

	public:
		virtual ~iRendererAPI() = default;

		virtual void Initialize() = 0;
		virtual void SetViewport(const int x, const int y, const int width, const int height) const noexcept = 0;
		virtual void Clear() = 0;
		virtual void DrawIndexed(const iVertexArray& vertexArray, uint32_t count = 0) = 0;
		virtual void SetClearColour(const Colour& colour)  = 0;

		inline static ApiType GetAPI() noexcept { return api; }

	private:
		virtual void OutputRenderingArchitecture() = 0;

	private:
		static ApiType api;
	};


}

