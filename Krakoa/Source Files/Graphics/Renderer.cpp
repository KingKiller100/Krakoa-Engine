﻿#include "Precompile.hpp"
#include "Renderer.hpp"
#include "RenderCommand.hpp"

#include "Renderer2D.hpp"
#include "ShaderLibrary.hpp"

#include "../Instrumentor.hpp"
#include "../Camera/OrthographicCamera.hpp"
#include "../Graphics/Rendering Resources/iShader.hpp"

namespace krakoa::graphics
{
	const kmaths::Matrix4x4f* Renderer::camera_VPMat = nullptr;

	void Renderer::Initialize(ShaderLibrary& shaderLibrary)
	{
		KRK_PROFILE_FUNCTION();
		KRK_BANNER("Rendering Architecture Info", "GRAPHICS");
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
		KRK_FATAL(!vertexArray.GetVertexBuffers().empty(), "No vertex buffer attached to this vertex array!");

		shader.Bind();
		shader.SetMat4x4("u_VpMat", *camera_VPMat);
		shader.SetMat4x4("u_TransformMat", transform);
		vertexArray.Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	void Renderer::Clear()
	{
		KRK_PROFILE_FUNCTION();
		RenderCommand::Clear();
	}

	void Renderer::SetClearColour(const kmaths::Vector4f& colour)
	{
		KRK_PROFILE_FUNCTION();
		RenderCommand::SetClearColour(colour);
	}

}