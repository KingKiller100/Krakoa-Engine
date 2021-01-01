#include "Precompile.hpp"
#include "Renderer.hpp"
#include "RenderCommand.hpp"

#include "2D/Renderer2D.hpp"
#include "ShaderLibrary.hpp"

#include "../Debug/Instrumentor.hpp"
#include "../Camera/OrthographicCamera.hpp"
#include "../Graphics/Resources/iShader.hpp"

namespace krakoa::graphics
{
	const kmaths::Matrix4x4f* Renderer::camera_VPMat = nullptr;

	void Renderer::Initialize(ShaderLibrary& shaderLibrary)
	{
		KRK_PROFILE_FUNCTION();
		KRK_BANNER("Architecture Info", "GRAPHICS", "*", "*", 10);
		RenderCommand::Initialize();
		Renderer2D::Initialize(shaderLibrary);
	}

	void Renderer::ShutDown()
	{
		KRK_PROFILE_FUNCTION();
		Renderer2D::ShutDown();
		delete ShaderLibrary::Pointer();
	}

	void Renderer::OnWindowResize(const int x, const int y, const int width, const int height) noexcept
	{
		KRK_PROFILE_FUNCTION();
		RenderCommand::OnWindowResize(x, y, width, height);
	}

	void Renderer::BeginScene(const OrthographicCamera& camera)
	{
		KRK_PROFILE_FUNCTION();
		camera_VPMat = &camera.GetViewProjectionMatrix();
	}

	void Renderer::EndScene()
	{}

	void Renderer::Submit(iShader& shader, const iVertexArray& vertexArray, const kmaths::TransformMatrix<float>& transform)
	{
		KRK_PROFILE_FUNCTION();
		KRK_ASSERT(!vertexArray.GetVertexBuffers().empty(), "No vertex buffer attached to this vertex array!");

		shader.Bind();
		shader.SetMat4x4("u_VpMat", *camera_VPMat);
		shader.SetMat4x4("u_TransformMat", transform);
		vertexArray.Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::Update()
	{
		KRK_PROFILE_FUNCTION();
		
#ifdef KRAKOA_DEBUG
		SetClearColour(colours::Orange); 
#else
		SetClearColour({ 0.05f, 0.05f, 0.05f, 1.f }); // Black background colour
#endif // DEBUG

		Clear();
	}

	void Renderer::Clear()
	{
		KRK_PROFILE_FUNCTION();
		RenderCommand::Clear();
	}

	void Renderer::SetClearColour(const Colour& colour)
	{
		KRK_PROFILE_FUNCTION();
		RenderCommand::SetClearColour(colour);
	}

}