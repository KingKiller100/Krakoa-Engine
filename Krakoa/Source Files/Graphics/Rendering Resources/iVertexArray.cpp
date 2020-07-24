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
		static bool firstEntry = true;

		iVertexArray* vArr = nullptr;
		
		switch (Renderer::GetAPI()) {
		case iRendererAPI::API::NONE:   KRK_FATAL(false, "RedererAPI::NONE not supported yet!"); break;
		case iRendererAPI::API::OPENGL: vArr =  new OpenGLVertexArray(); break;
		default:                        KRK_FATAL(false, "Unknown RendererAPI type!");
		}

		if (firstEntry)
		{
			firstEntry = false;
			AddToParent("Renderer");
		}
		
		return vArr;
	}


}
