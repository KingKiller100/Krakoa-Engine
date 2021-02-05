#include "Precompile.hpp"
#include "iTexture2D.hpp"

#include "../../Renderer.hpp"

#include "../../../Debug/Debug.hpp"
#include "../../../Logging/EngineLogger.hpp"
#include "../../../Platform//OpenGL/OpenGLTexture2D.hpp"
#include "../../../FileSystem/VirtualFileExplorer.hpp"

// #include <Utility/String/kStringTricks.hpp>


namespace krakoa::graphics
{
	iTexture2D* iTexture2D::Create(const uint32_t width, const uint32_t height)
	{
		switch (Renderer::GetAPI()) {
		case iRendererAPI::API::NONE:   KRK_ERR("RedererAPI::NONE not supported yet!"); break;
		case iRendererAPI::API::OPENGL: return new OpenGLTexture2D(width, height);
		default:                            KRK_FATAL("Unknown RendererAPI type!");
		}

		return nullptr;
	}

	iTexture2D* iTexture2D::Create(const std::filesystem::path& path, const bool cache)
	{
		const auto texturesPath = filesystem::VirtualFileExplorer::GetRealPath("Textures");
		
		switch (Renderer::GetAPI()) {
		case iRendererAPI::API::NONE:   KRK_ERR("RedererAPI::NONE not supported yet!"); break;
		case iRendererAPI::API::OPENGL: return new OpenGLTexture2D(texturesPath / path, cache);
		default:                            KRK_FATAL("Unknown RendererAPI type!");
		}

		return nullptr;
	}
}
