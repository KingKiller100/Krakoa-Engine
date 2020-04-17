#pragma once

#include "iRendererAPI.hpp"

#include <Maths/Matrices/MatrixMathsHelper.hpp>

namespace krakoa { class OrthographicCamera; }

namespace krakoa::graphics
{
	class iShader;

	class Renderer
	{
	public:
		static void Initialize();
		static void ShutDown();

		static void OnWindowResize(const float x, const float y, const float width, const float height)  noexcept;

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();
		static void Submit(iShader& shader, const iVertexArray& vertexArray, const kmaths::TransformMatrix<float>& transform = kmaths::GetTransformIdentity<float>());

		static void Clear();
		static void SetClearColour(const kmaths::Vector4f& colour);

		static inline iRendererAPI::API GetAPI()  noexcept { return iRendererAPI::GetAPI(); }

	private:
		static const kmaths::Matrix4x4f* camera_VPMat;
	};
}