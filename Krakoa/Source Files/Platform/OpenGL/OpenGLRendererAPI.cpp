#include "Precompile.hpp"
#include "OpenGLRendererAPI.hpp"

#include "../../Core/Logging/CoreLogger.hpp"

#include <GLAD/glad.h>

namespace krakoa::graphics
{
	void OpenGLRendererAPI::Initialize()
	{
		OutputRenderingArchitecture();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void OpenGLRendererAPI::OutputRenderingArchitecture()
	{
		// Rendering hardware info
		KRK_INFO("API: OpenGL");
		KRK_INFO(klib::kFormat::ToString("Vendor: %s", glGetString(GL_VENDOR)));
		KRK_INFO(klib::kFormat::ToString("Renderer: %s", glGetString(GL_RENDERER)));
		KRK_INFO(klib::kFormat::ToString("Version: %s", glGetString(GL_VERSION)));
	}

	void OpenGLRendererAPI::SetClearColour(const kmaths::Vector4f& colour)
	{
		const auto& r = colour.X();
		const auto& g = colour.Y();
		const auto& b = colour.Z();
		const auto& a = colour.W();

		glClearColor(r, g, b, a);
	}

	void OpenGLRendererAPI::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const iVertexArray& vertexArray)
	{
		glDrawElements(
			GL_TRIANGLES,
			vertexArray.GetIndexBuffer()->GetCount(),
			GL_UNSIGNED_INT, 
			nullptr
		);
	}
}