#include "Precompile.hpp"
#include "Renderer.hpp"
#include "RenderCommand.hpp"

#include "Renderer2D.hpp"
#include "../Camera/OrthographicCamera.hpp"
#include "../Rendering/Rendering Resources/iShader.hpp"

namespace krakoa::graphics
{
	const kmaths::Matrix4x4f* Renderer::camera_VPMat = nullptr;

	void Renderer::Initialize()
	{
		KRK_BANNER("Rendering Architecture Info", "GRAPHICS");
		RenderCommand::Initialize();
		graphics::Renderer2D::Initialize();
	}

	void Renderer::ShutDown()
	{
		graphics::Renderer2D::ShutDown();
	}

	void Renderer::OnWindowResize(const float x, const float y, const float width, const float height) noexcept
	{
		RenderCommand::OnWindowResize(x, y, width, height);
	}

	void Renderer::BeginScene(const OrthographicCamera& camera)
	{
		camera_VPMat = &camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{}

	void Renderer::Submit(iShader& shader, const iVertexArray& vertexArray, const kmaths::TransformMatrix<float>& transform)
	{
		KRK_FATAL(!vertexArray.GetVertexBuffers().empty(), "No vertex buffer attached to this vertex array!");

		shader.Bind();
		shader.UploadUniformMatrix4x4("u_VpMat", *camera_VPMat);
		shader.UploadUniformMatrix4x4("u_TransformMat", transform);
		vertexArray.Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::Clear()
	{
		RenderCommand::Clear();
	}

	void Renderer::SetClearColour(const kmaths::Vector4f& colour)
	{
		RenderCommand::SetClearColour(colour);
	}

}