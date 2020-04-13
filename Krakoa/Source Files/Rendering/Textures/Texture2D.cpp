#include "Precompile.hpp"
#include "Texture2D.hpp"

#include "../Renderer.hpp"

#include "../../Platform//OpenGL/OpenGLTexture.hpp"
#include "../../Core/Logging/CoreLogger.hpp"

namespace krakoa::graphics
{
	Texture2D* Texture2D::Create(const std::string_view & path)
	{
		switch (Renderer::Reference().GetAPI()) {
		case iRendererAPI::API::NONE:   KRK_FATAL(false, "RedererAPI::NONE not supported yet!"); break;
		case iRendererAPI::API::OPENGL: return new OpenGLTexture2D(path);
		default:                        KRK_FATAL(false, "Unknown RendererAPI type!");
		}

		return nullptr;
	}
}