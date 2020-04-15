#include "Precompile.hpp"
#include "iTexture2D.hpp"

#include "../Renderer.hpp"

#include "../../Platform//OpenGL/OpenGLTexture2D.hpp"
#include "../../Core/Logging/CoreLogger.hpp"

namespace krakoa::graphics
{
	iTexture2D* iTexture2D::Create(const std::string_view & path, const bool cache)
	{
		switch (Renderer::Reference().GetAPI()) {
		case iRendererAPI::API::NONE:   KRK_FATAL(false, "RedererAPI::NONE not supported yet!"); break;
		case iRendererAPI::API::OPENGL: return new OpenGLTexture2D(path, cache);
		default:                        KRK_FATAL(false, "Unknown RendererAPI type!");
		}

		return nullptr;
	}
}