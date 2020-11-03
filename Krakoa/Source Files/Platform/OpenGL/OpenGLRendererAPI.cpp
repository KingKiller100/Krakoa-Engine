#include "Precompile.hpp"
#include "OpenGLRendererAPI.hpp"

#include "../../Instrumentor.hpp"
#include "../../Core/Logging/CoreLogger.hpp"

#include <Utility/String/kToString.hpp>

#include <imgui.h>
#include <GLFW/glfw3.h>
#include <GLAD/glad.h>

namespace krakoa::graphics
{
	static std::string_view s_GlVersion ;
	static std::string_view s_GlVendor;
	static std::string_view s_GlRenderer;
	static std::string_view s_GlfwVersion;
	static std::string_view s_ImGuiVersion;
	
	void OpenGLRendererAPI::Initialize()
	{
		InitRendererInfo();
		OutputRenderingArchitecture();

		KRK_PROFILE_FUNCTION();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);

		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
		{
			const auto msgType = klib::kString::ToString("OpenGL %s:",
				type == GL_DEBUG_TYPE_ERROR
				? "ERROR"
				: "CALLBACK");
			const auto msgFormat = msgType +
				"\n type = 0x%08x\n severity = 0x%08x\n message = %s\n";

			if (type == GL_DEBUG_TYPE_ERROR)
			{
				KRK_ERRR(klib::kString::ToString(msgFormat,
					type,
					severity,
					message));
			}
			else
			{
				KRK_DBUG(klib::kString::ToString(msgFormat,
					type,
					severity,
					message));
			}
		},
			nullptr);

	}

	void OpenGLRendererAPI::InitRendererInfo() noexcept
	{
		s_GlVersion = REINTERPRET(const char*, glGetString(GL_VERSION));
		s_GlVendor = REINTERPRET(const char*, glGetString(GL_VENDOR));
		s_GlRenderer = REINTERPRET(const char*, glGetString(GL_RENDERER));
		s_GlfwVersion = glfwGetVersionString();
		s_ImGuiVersion = IMGUI_VERSION;
	}


	void OpenGLRendererAPI::OutputRenderingArchitecture()
	{
		KRK_PROFILE_FUNCTION();

		// Rendering hardware info
		KRK_INFO("API: OpenGL");
		KRK_INFO(klib::kString::ToString("Version: %s",       s_GlVersion));
		KRK_INFO(klib::kString::ToString("Vendor: %s",        s_GlVendor));
		KRK_INFO(klib::kString::ToString("Hardware: %s",      s_GlRenderer));
		KRK_INFO(klib::kString::ToString("GLFW Version: %s",  s_GlfwVersion));
		KRK_INFO(klib::kString::ToString("ImGui Version: %s", s_ImGuiVersion));
	}

	void OpenGLRendererAPI::SetClearColour(const kmaths::Vector4f& colour)
	{
		KRK_PROFILE_FUNCTION();

		const auto r = colour.X();
		const auto g = colour.Y();
		const auto b = colour.Z();
		const auto a = colour.W();

		glClearColor(r, g, b, a);
	}

	void OpenGLRendererAPI::Clear()
	{
		KRK_PROFILE_FUNCTION();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndexed(const iVertexArray& vertexArray, uint32_t count)
	{
		KRK_PROFILE_FUNCTION();

		if (count == 0)
			count = vertexArray.GetIndexBuffer()->GetCount();


		glDrawElements(
			GL_TRIANGLES,
			count,
			GL_UNSIGNED_INT,
			nullptr
		);
	}

	void OpenGLRendererAPI::SetViewport(const int x, const int y, const int width, const int height) const noexcept
	{
		KRK_PROFILE_FUNCTION();
		glViewport(x, y, width, height);
	}

}