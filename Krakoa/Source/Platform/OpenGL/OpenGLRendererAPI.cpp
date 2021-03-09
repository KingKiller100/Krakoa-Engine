#include "Precompile.hpp"
#include "OpenGLRendererAPI.hpp"

#include "../../Debug/Instrumentor.hpp"
#include "../../Logging/EngineLogger.hpp"
#include "../../Graphics/Colour.hpp"

#include <Utility/String/kToString.hpp>

#include <imgui.h>
#include <GLFW/glfw3.h>
#include <GLAD/glad.h>

namespace krakoa::graphics
{
	namespace
	{
		struct RendererInfo
		{
			std::string_view glVersion;
			std::string_view glVendor;
			std::string_view glRenderer;
			std::string_view glfwVersion;
			std::string_view imGuiVersion;
		} g_RendererInfo;
	}

	OpenGLRendererAPI::~OpenGLRendererAPI()
		= default;

	void OpenGLRendererAPI::Initialize()
	{
		api = API::OPENGL;

		InitRendererInfo();
		OutputRenderingArchitecture();

		KRK_PROFILE_FUNCTION();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);

		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback([](GLenum, GLenum type, GLuint, GLenum severity, GLsizei
			, const GLchar* message, const void*)
			{
				const auto isError = type == GL_DEBUG_TYPE_ERROR;

				const auto msgFormat = "OpenGL {0}:\n type = 0x{1:8h}\n severity = 0x{2:8h}\n message = {3}\n";

				if (isError)
				{
					KRK_ERR(klib::kString::ToString(msgFormat,
						"ERROR",
						type,
						severity,
						message));
				}
				else
				{
					KRK_DBG(klib::kString::ToString(msgFormat,
						"CALLBACK",
						type,
						severity,
						message));
				}
			},
			nullptr);

	}

	void OpenGLRendererAPI::InitRendererInfo() noexcept
	{
		g_RendererInfo.glVersion = REINTERPRET(const char*, glGetString(GL_VERSION));
		g_RendererInfo.glVendor = REINTERPRET(const char*, glGetString(GL_VENDOR));
		g_RendererInfo.glRenderer = REINTERPRET(const char*, glGetString(GL_RENDERER));
		g_RendererInfo.glfwVersion = glfwGetVersionString();
		g_RendererInfo.imGuiVersion = IMGUI_VERSION;
	}


	void OpenGLRendererAPI::OutputRenderingArchitecture()
	{
		KRK_PROFILE_FUNCTION();

		// Rendering hardware info
		KRK_INF(klib::ToString("API: {0}", api.ToString()));
		KRK_INF(klib::ToString("Version: {0}", g_RendererInfo.glVersion));
		KRK_INF(klib::ToString("Vendor: {0}", g_RendererInfo.glVendor));
		KRK_INF(klib::ToString("Hardware: {0}", g_RendererInfo.glRenderer));
		KRK_INF(klib::ToString("GLFW Version: {0}", g_RendererInfo.glfwVersion));
		KRK_INF(klib::ToString("ImGui Version: {0}", g_RendererInfo.imGuiVersion));
	}

	void OpenGLRendererAPI::SetClearColour(const Colour& colour)
	{
		KRK_PROFILE_FUNCTION();

		const auto c = colour.ToFloats<float>();

		const auto r = c[0];
		const auto g = c[1];
		const auto b = c[2];
		const auto a = c[3];

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