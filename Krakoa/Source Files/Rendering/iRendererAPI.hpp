#pragma once

#include "Rendering Resources/iVertexArray.hpp"

#include <Maths/Vectors/Vector4.hpp>

namespace krakoa::graphics
{
	class iRendererAPI
	{
	public:
	enum class API : unsigned short
	{
		NONE = 0,
		OPENGL,
	};

	public:
		virtual void Initialize() = 0;
		virtual void SetViewport(const float x, const float y, const float width, const float height) const noexcept = 0;
		virtual void Clear() = 0;
		virtual void DrawIndexed(const iVertexArray& vertexArray) = 0;
		virtual void SetClearColour(const kmaths::Vector4f& colour)  = 0;

		inline static API GetAPI() noexcept {return api;}

	private:
		virtual void OutputRenderingArchitecture() = 0;

	private:
		static API api;
	};


}

