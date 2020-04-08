#include "Precompile.hpp"
#include "Renderer.hpp"
#include "RenderCommand.hpp"


namespace krakoa::graphics
{
	Renderer::Renderer(Token &&)
	{
		KRK_BANNER("Rendering Architecture Info", "GRAPHICS");
		RenderCommand::OutputRenderingArchitecture();
	}

	Renderer::~Renderer()
		= default;

	void Renderer::BeginScene() const
	{

	}

	void Renderer::EndScene() const
	{

	}

	void Renderer::Submit(const iVertexArray& vertexArray) const
	{
		vertexArray.Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}