﻿#pragma once
#include "Rendering Resources/iVertexArray.hpp"
#include "iRendererAPI.hpp"

namespace krakoa::graphics
{
	class RenderCommand
	{
	public:
		inline static void Initialize()
		{
			rendererAPI->Initialize();
		}

		inline static void OnWindowResize(const int x, const int y, const int width, const int height) noexcept
		{
			rendererAPI->SetViewport(x, y, width, height);
		}

		inline static void Clear()
		{
			rendererAPI->Clear(); 
		}

		inline static void SetClearColour(const kmaths::Vector4f& colour) 
		{
			rendererAPI->SetClearColour(colour);
		}

		inline static void DrawIndexed(const iVertexArray& vertexArray) 
		{
			rendererAPI->DrawIndexed(vertexArray);
		}

	private:
		static iRendererAPI* rendererAPI;
	};
	 

}