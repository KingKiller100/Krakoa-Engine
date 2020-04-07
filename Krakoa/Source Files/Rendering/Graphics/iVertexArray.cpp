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
		switch (Renderer::Reference().GetAPI()) {
		case RendererAPI::NONE: KRK_FATAL(false, "RedererAPI::NONE not supported yet!");
		case RendererAPI::OPENGL: return new OpenGLVertexArray();
		default:  KRK_FATAL(false, "Unknown RendererAPI type!");
		}
		return nullptr;
	}


}
