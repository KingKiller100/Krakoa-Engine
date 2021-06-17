#pragma once

#include "iRendererAPI.hpp"

#include <Maths/Matrices/MatrixMathsHelper.hpp>

namespace krakoa { class OrthographicCamera; }

namespace krakoa::gfx
{
	class iShader;
	class ShaderLibrary;

	class Renderer
	{
	public:
		static void Initialize();
		static void ShutDown();

		static void OnWindowResize(const int x, const int y, const int width, const int height)  noexcept;

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		static void Submit(iShader& shader, const iVertexArray& vertexArray
			, const kmaths::TransformMatrix<float>& transform = kmaths::GetTransformIdentity<float>());

		static void Update();
		
		static void Clear();
		static void SetClearColour(const Colour& colour);

		static inline iRendererAPI::API GetAPI()  noexcept { return iRendererAPI::GetAPI(); }

	private:
		static ShaderLibrary s_ShaderLib;
	};
}