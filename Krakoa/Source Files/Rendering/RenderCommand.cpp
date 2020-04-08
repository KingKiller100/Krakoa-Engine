#include "Precompile.hpp"
#include "RenderCommand.hpp"

#include "../Platform/OpenGL/OpenGLRendererAPI.hpp"


namespace krakoa::graphics
{
	iRendererAPI* RenderCommand::rendererAPI = new OpenGLRendererAPI();


}
