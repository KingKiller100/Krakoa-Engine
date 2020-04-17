#pragma once

#include "iRendererAPI.hpp"

#include "../Patterns/SimpleSingleton.hpp"

#include <Maths/Matrices/MatrixMathsHelper.hpp>

namespace krakoa { class OrthographicCamera; }

namespace krakoa::graphics
{
	class iShader;

	class Renderer : public pattern::SimpleSingleton<Renderer>
	{
	public:
		Renderer(Token&&);
		~Renderer();

		void OnWindowResize(const float x, const float y, const float width, const float height) const noexcept;

		void BeginScene(const OrthographicCamera& camera);
		void EndScene() const;
		void Submit(iShader& shader, const iVertexArray& vertexArray, const kmaths::TransformMatrix<float>& transform = kmaths::GetTransformIdentity<float>()) const;
		 
		void Clear() const;
		void SetClearColour(const kmaths::Vector4f& colour) const;

		inline iRendererAPI::API GetAPI() const noexcept { return iRendererAPI::GetAPI(); }

	private:
		const kmaths::Matrix4x4f* camera_VPMat;
	};
}