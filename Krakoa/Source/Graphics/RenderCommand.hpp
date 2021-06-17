#pragma once
#include "iRendererAPI.hpp"
#include "Resources/iVertexArray.hpp"
#include "../Core/PointerTypes.hpp"

namespace krakoa::gfx
{
	class RenderCommand
	{
	public:
		static void Initialize();
		static void ShutDown();

		static void OnWindowResize(const int x, const int y, const int width, const int height) noexcept;

		static void Clear();

		static void SetClearColour(const Colour& colour);

		static void DrawIndexed(const iVertexArray& vertexArray, uint32_t count = 0);

	private:
		static void CreateApi();
		
	private:
		static Solo_Ptr<iRendererAPI> rendererAPI;
	};
	 

}