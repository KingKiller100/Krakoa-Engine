#pragma once

#include "iRendererAPI.hpp"

#include <Maths/Matrices/MatrixMathsHelper.hpp>

namespace krakoa { class OrthographicCamera; }

namespace krakoa::graphics
{
	class iShader;
	class ShaderLibrary;

	class Renderer
	{
	public:
		static void Initialize(ShaderLibrary& shaderLibrary);
		static void ShutDown();

		static void OnWindowResize(const int x, const int y, const int width, const int height)  noexcept;

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		static void Submit(iShader& shader, const iVertexArray& vertexArray, const kmaths::TransformMatrix<float>& transform = kmaths::GetTransformIdentity<float>());

		static void Update();
		
		static void Clear();
		static void SetClearColour(const kmaths::Vector4f& colour);

		static inline iRendererAPI::API GetAPI()  noexcept { return iRendererAPI::GetAPI(); }

	private:
		static const kmaths::Matrix4x4f* camera_VPMat;
	};
}