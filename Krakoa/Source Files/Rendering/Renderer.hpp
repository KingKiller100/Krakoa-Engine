#pragma once

#include "iRendererAPI.hpp"

#include "../Camera/OrthographicCamera.hpp"

#include "../Patterns/SimpleSingleton.hpp"

namespace krakoa::graphics
{
	class iShader;

	class Renderer : public pattern::SimpleSingleton<Renderer>
	{
	public:
		Renderer(Token&&);
		~Renderer();
		 
		void BeginScene(OrthographicCamera& camera);
		void EndScene() const;
		void Submit(iShader& shader, const iVertexArray& vertexArray) const;

		inline iRendererAPI::API GetAPI() const noexcept { return iRendererAPI::GetAPI(); }

	private:
		const kmaths::Matrix4x4f* camera_VPMat;
	};
}