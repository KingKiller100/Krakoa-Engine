#include "Precompile.hpp"
#include "iBuffers.hpp"

#include "../../Debug/Debug.hpp"
#include "../../Platform/OpenGL/OpenGLBuffers.hpp"
#include "../Renderer.hpp"


namespace krakoa::graphics
{
	iVertexBuffer::~iVertexBuffer()
		= default;

	iVertexBuffer * iVertexBuffer::Create(uint32_t size)
	{
		switch (Renderer::GetAPI()) {
		case iRendererAPI::API::NONE:   KRK_ERR("RedererAPI::NONE not supported yet!"); break;
		case iRendererAPI::API::OPENGL: return new OpenGLVertexBuffer(size);
		default:                            KRK_FATAL("Unknown RendererAPI type!");
		}
		return nullptr;
	}

	iVertexBuffer * iVertexBuffer::Create(float * vertices, uint32_t size)
	{
		switch (Renderer::GetAPI()) {
		case iRendererAPI::API::NONE:   KRK_ERR("RedererAPI::NONE not supported yet!"); break;
		case iRendererAPI::API::OPENGL: return new OpenGLVertexBuffer(vertices, size);
		default:                            KRK_FATAL("Unknown RendererAPI type!");
		}
		return nullptr;
	}

	iIndexBuffer::~iIndexBuffer()
		= default;

	iIndexBuffer * iIndexBuffer::Create(uint32_t * indices, uint32_t count)
	{
		switch (Renderer::GetAPI()) {
		case iRendererAPI::API::NONE:   KRK_ERR("RedererAPI::NONE not supported yet!"); break;
		case iRendererAPI::API::OPENGL: return new OpenGLIndexBuffer(indices, count);
		default:                            KRK_FATAL("Unknown RendererAPI type!");
		}
		return nullptr;
	}
}
