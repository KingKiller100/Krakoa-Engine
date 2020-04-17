#pragma once
#include "../../Rendering/iRendererAPI.hpp"

namespace krakoa::graphics
{
	class OpenGLRendererAPI : public iRendererAPI
	{
	public:
		void Initialize() override;

		void Clear() override;
		void DrawIndexed(const iVertexArray& vertexArray) override;
		void SetClearColour(const kmaths::Vector4f& colour) override;

		void SetViewport(const float x, const float y, const float width, const float height) const noexcept override;

	private:
		void OutputRenderingArchitecture() override;
	};

}
