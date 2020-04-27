#pragma once
#include "../../Rendering/iRendererAPI.hpp"

namespace krakoa::graphics
{
	class OpenGLRendererAPI : public iRendererAPI
	{
	public:
		void Initialize() override;

		void Clear() override;
		void DrawIndexed(const iVertexArray& vertexArray, uint32_t count = 0) override;
		void SetClearColour(const kmaths::Vector4f& colour) override;

		void SetViewport(const int x, const int y, const int width, const int height) const noexcept override;

	private:
		void OutputRenderingArchitecture() override;
	};

}
