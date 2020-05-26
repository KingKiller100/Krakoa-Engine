#include "Precompile.hpp"
#include "iVertexArray.hpp"

#include "../../Core/Logging/CoreLogger.hpp"

#include "../../Platform/OpenGL/OpenGLVertexArray.hpp"
#include "../Renderer.hpp"

namespace krakoa::graphics
{
	iVertexArray::~iVertexArray()
		= default;

	iVertexArray * iVertexArray::Create()
	{
		switch (Renderer::GetAPI()) {
		case iRendererAPI::API::NONE:   KRK_FATAL(false, "RedererAPI::NONE not supported yet!"); break;
		case iRendererAPI::API::OPENGL: return new OpenGLVertexArray();
		default:                        KRK_FATAL(false, "Unknown RendererAPI type!");
		}
		return nullptr;
	}


}
