﻿#pragma once

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
		virtual void OutputRenderingArchitecture() = 0;
		virtual void Clear() = 0;
		virtual void DrawIndexed(const iVertexArray& vertexArray) = 0;
		virtual void SetClearColour(const kmaths::Vector4f& colour)  = 0;

		inline static API GetAPI() noexcept {return api;}

	private:
		static API api;
	};


}

