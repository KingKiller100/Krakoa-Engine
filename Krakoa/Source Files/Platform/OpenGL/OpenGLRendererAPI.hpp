#pragma once
#include "../../Rendering/iRendererAPI.hpp"

namespace krakoa::graphics
{
	class OpenGLRendererAPI : public iRendererAPI
	{
	public:
		void OutputRenderingArchitecture() override;

		void Clear() override;
		void DrawIndexed(const iVertexArray& vertexArray) override;
		void SetClearColour(const kmaths::Vector4f& colour) override;
	};

}
