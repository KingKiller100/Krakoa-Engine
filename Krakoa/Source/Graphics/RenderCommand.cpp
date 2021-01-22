#include "Precompile.hpp"
#include "RenderCommand.hpp"

#include "../Config/GlobalConfig.hpp"

#include "../Platform/OpenGL/OpenGLRendererAPI.hpp"

namespace krakoa::graphics
{
	Solo_Ptr<iRendererAPI> RenderCommand::rendererAPI = nullptr;

	void RenderCommand::Initialize()
	{
		CreateApi();
		rendererAPI->Initialize();
	}

	void RenderCommand::CreateApi()
	{
		const auto& globalConfig = configuration::GlobalConfig::Reference();
		const auto apiStr = globalConfig.Get<std::string>("Renderer", "API");
		const auto api = iRendererAPI::API::FromString(klib::ToUpper(apiStr));

		switch (api.ToEnum()) {
		case iRendererAPI::API::OPENGL: rendererAPI = Make_Solo<OpenGLRendererAPI>(); break;
		case iRendererAPI::API::NONE:
		default: KRK_FATAL("Rendering api must be set!"); // Temp 'til we figure out how to do headless mode
			break;
		}
	}

	void RenderCommand::ShutDown()
	{
		rendererAPI.reset();
	}

	void RenderCommand::OnWindowResize(const int x, const int y, const int width, const int height) noexcept
	{
		rendererAPI->SetViewport(x, y, width, height);
	}

	void RenderCommand::Clear()
	{
		rendererAPI->Clear();
	}

	void RenderCommand::SetClearColour(const Colour& colour)
	{
		rendererAPI->SetClearColour(colour);
	}

	void RenderCommand::DrawIndexed(const iVertexArray& vertexArray, uint32_t count)
	{
		rendererAPI->DrawIndexed(vertexArray, count);
	}
}
