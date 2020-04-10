#include "Precompile.hpp"
#include "iShader.hpp"

#include "../Renderer.hpp"

#include "../../Core/Logging/CoreLogger.hpp"
#include "../../Platform/OpenGL/OpenGLShader.hpp"

namespace krakoa::graphics
{
	iShader::~iShader()
		= default;

	iShader * iShader::Create(const std::string_view & vertexSource, const std::string_view & fragmentSource)
	{
		switch (Renderer::Reference().GetAPI()) {
		case iRendererAPI::API::NONE:   KRK_FATAL(false, "RedererAPI::NONE not supported yet!"); break;
		case iRendererAPI::API::OPENGL: return new OpenGLShader(vertexSource, fragmentSource);
		default:  KRK_FATAL(false, "Unknown RendererAPI type!");
		}
		return nullptr;
	}
}