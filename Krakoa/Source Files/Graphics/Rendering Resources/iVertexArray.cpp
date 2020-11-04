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
		case iRendererAPI::ApiType::NONE:   KRK_ASSERT(false, "RedererAPI::NONE not supported yet!"); break;
		case iRendererAPI::ApiType::OPENGL: return new OpenGLVertexArray(); break;
		default:                        KRK_ASSERT(false, "Unknown RendererAPI type!");
		}
	}


}
