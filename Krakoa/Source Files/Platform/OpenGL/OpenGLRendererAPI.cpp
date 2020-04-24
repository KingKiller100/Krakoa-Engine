#include "Precompile.hpp"
#include "OpenGLRendererAPI.hpp"

#include "../../Core/Logging/CoreLogger.hpp"

#include <Utility/Format/kFormatToString.hpp>

#include <imgui.h>
#include <GLFW/glfw3.h>
#include <GLAD/glad.h>

namespace krakoa::graphics
{
	void OpenGLRendererAPI::Initialize()
	{
		OutputRenderingArchitecture();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
			{
				const auto msgType = klib::kFormat::ToString("OpenGL %s:",
					type == GL_DEBUG_TYPE_ERROR
					? "ERROR"
					: "CALLBACK");
				auto msgFormat = msgType +
					"\n type = 0x%08x\n severity = 0x%08x\n message = %s\n";

				if (type == GL_DEBUG_TYPE_ERROR)
				{
					KRK_ERRR(klib::kFormat::ToString(msgFormat,
						type,
						severity,
						message));
				}
				else
				{
					KRK_DBUG(klib::kFormat::ToString(msgFormat,
						type,
						severity,
						message));
				}
			},
			nullptr);

		glEnable(GL_DEPTH_TEST);

	}

	void OpenGLRendererAPI::OutputRenderingArchitecture()
	{
		// Rendering hardware info
		KRK_INFO("API: OpenGL");
		KRK_INFO(klib::kFormat::ToString("Version: %s", glGetString(GL_VERSION)));
		KRK_INFO(klib::kFormat::ToString("Vendor: %s", glGetString(GL_VENDOR)));
		KRK_INFO(klib::kFormat::ToString("Hardware: %s", glGetString(GL_RENDERER)));
		KRK_INFO(klib::kFormat::ToString("GLFW Version: %s", glfwGetVersionString()));
		KRK_INFO(klib::kFormat::ToString("ImGui Version: %s", IMGUI_VERSION));
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

	void OpenGLRendererAPI::SetViewport(const int x, const int y, const int width, const int height) const noexcept
	{
		glViewport(x, y, width, height);
	}

}