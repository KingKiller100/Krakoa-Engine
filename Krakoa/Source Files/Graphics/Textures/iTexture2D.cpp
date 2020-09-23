﻿#include "Precompile.hpp"
#include "iTexture2D.hpp"

#include "../Renderer.hpp"

#include "../../Platform//OpenGL/OpenGLTexture2D.hpp"
#include "../../Core/Logging/CoreLogger.hpp"

#include <Utility/String/kStringManipulation.hpp>

namespace krakoa::graphics
{
	iTexture2D * iTexture2D::Create(const uint32_t width, const uint32_t height, const bool cache)
	{
		switch (Renderer::GetAPI()) {
		case iRendererAPI::API::NONE:   KRK_FATAL(false, "RedererAPI::NONE not supported yet!"); break;
		case iRendererAPI::API::OPENGL: return new OpenGLTexture2D(width, height, cache);
		default:                        KRK_FATAL(false, "Unknown RendererAPI type!");
		}

		return nullptr;
	}

	iTexture2D* iTexture2D::Create(const std::string_view & path, const bool cache)
	{
		const auto formattedPath = klib::kString::Replace(path, '/', '\\');

		switch (Renderer::GetAPI()) {
		case iRendererAPI::API::NONE:   KRK_FATAL(false, "RedererAPI::NONE not supported yet!"); break;
		case iRendererAPI::API::OPENGL: return new OpenGLTexture2D(formattedPath, cache);
		default:                        KRK_FATAL(false, "Unknown RendererAPI type!");
		}

		return nullptr;
	}
}