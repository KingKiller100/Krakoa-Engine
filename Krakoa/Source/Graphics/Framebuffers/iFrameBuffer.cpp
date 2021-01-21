#include "Precompile.hpp"
#include "iFrameBuffer.hpp"

#include "../Renderer.hpp"

#include "../../Debug/Debug.hpp"
#include "../../Platform/OpenGL/OpenGLFrameBuffer.hpp"

namespace krakoa::graphics
{
	iFrameBuffer* iFrameBuffer::Create(const FrameBufferSpecification& spec)
	{
		switch (Renderer::GetAPI()) {
		case iRendererAPI::API::NONE:   KRK_ERR("RedererAPI::NONE not supported yet!"); break;
		case iRendererAPI::API::OPENGL: return new OpenGLFrameBuffer(spec);
		default:                            KRK_FATAL("Unknown RendererAPI type!");
		}
		
		return nullptr;
	}
}
