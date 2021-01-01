#include "Precompile.hpp"
#include "iVertexArray.hpp"

#include "../../Debug/Debug.hpp"

#include "../../Platform/OpenGL/OpenGLVertexArray.hpp"
#include "../Renderer.hpp"


namespace krakoa::graphics
{
	iVertexArray::~iVertexArray()
		= default;

	iVertexArray * iVertexArray::Create()
	{
		switch (Renderer::GetAPI()) {
		case iRendererAPI::ApiType::NONE:   KRK_ERR("RedererAPI::NONE not supported yet!"); break;
		case iRendererAPI::ApiType::OPENGL: return new OpenGLVertexArray();
		default:                            KRK_FATAL("Unknown RendererAPI type!");
		}
		return nullptr;
	}


}
