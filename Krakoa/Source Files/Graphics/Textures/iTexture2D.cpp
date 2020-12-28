#include "Precompile.hpp"
#include "iTexture2D.hpp"

#include "../Renderer.hpp"

#include "../../Debug/Debug.hpp"
#include "../../Logging/CoreLogger.hpp"
#include "../../Platform//OpenGL/OpenGLTexture2D.hpp"

#include <Utility/String/kStringTricks.hpp>


namespace krakoa::graphics
{
	iTexture2D* iTexture2D::Create(const uint32_t width, const uint32_t height)
	{
		switch (Renderer::GetAPI()) {
		case iRendererAPI::ApiType::NONE:   KRK_ERRR("RedererAPI::NONE not supported yet!"); break;
		case iRendererAPI::ApiType::OPENGL: return new OpenGLTexture2D(width, height);
		default:                            KRK_FATAL("Unknown RendererAPI type!");
		}

		return nullptr;
	}

	iTexture2D* iTexture2D::Create(const std::string_view& path, const bool cache)
	{
		const auto formattedPath = klib::kString::Replace(path, '/', '\\');

		switch (Renderer::GetAPI()) {
		case iRendererAPI::ApiType::NONE:   KRK_ERRR("RedererAPI::NONE not supported yet!"); break;
		case iRendererAPI::ApiType::OPENGL: return new OpenGLTexture2D(formattedPath, cache);
		default:                            KRK_FALSE("Unknown RendererAPI type!");
		}

		return nullptr;
	}
}
