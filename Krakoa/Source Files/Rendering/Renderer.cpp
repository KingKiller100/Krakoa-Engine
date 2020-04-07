#include "Precompile.hpp"
#include "Renderer.hpp"


namespace krakoa::graphics
{
	Renderer::Renderer(Token &&)
		: rendererAPI(RendererAPI::OPENGL)
	{}

	Renderer::~Renderer()
		= default;
}