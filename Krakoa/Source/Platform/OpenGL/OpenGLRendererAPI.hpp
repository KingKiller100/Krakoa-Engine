#pragma once
#include "../../Graphics/iRendererAPI.hpp"

namespace krakoa::gfx
{
	class OpenGLRendererAPI : public iRendererAPI
	{
	public:
		~OpenGLRendererAPI() override;
		void Initialize() override;

		void Clear() override;
		void DrawIndexed(const iVertexArray& vertexArray, uint32_t count = 0) override;
		void SetClearColour(const Colour& colour) override;

		void SetViewport(const int x, const int y, const int width, const int height) const noexcept override;

	private:
		void InitRendererInfo() noexcept;
		void OutputRenderingArchitecture() override;
	};

}
