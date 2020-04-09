#include "Precompile.hpp"
#include "Renderer.hpp"
#include "RenderCommand.hpp"

#include "../Rendering/Graphics/iShader.hpp"

namespace krakoa::graphics
{
	Renderer::Renderer(Token &&)
	{
		KRK_BANNER("Rendering Architecture Info", "GRAPHICS");
		RenderCommand::OutputRenderingArchitecture();
	}

	Renderer::~Renderer()
		= default;

	void Renderer::BeginScene(OrthographicCamera& camera)
	{
		camera_VPMat = &camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene() const
	{

	}

	void Renderer::Submit(iShader& shader, const iVertexArray& vertexArray) const
	{
		shader.Bind();
		shader.UploadUniformMatrix4x4("u_vpMat", *camera_VPMat);
		vertexArray.Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

}