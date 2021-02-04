#include "Precompile.hpp"
#include "iShader.hpp"

#include "../Renderer.hpp"

#include "../../Debug/Debug.hpp"
#include "../../Logging/EngineLogger.hpp"
#include "../../Platform/OpenGL/OpenGLShader.hpp"

#include <Utility/FileSystem/kFileSystem.hpp>
#include <Utility/String/Tricks/kStringOperators.hpp>

using namespace klib::kString::operators;

namespace krakoa::graphics
{
	iShader::~iShader()
		= default;

	iShader * iShader::Create(const std::string_view& name, const std::filesystem::path& shaderFilePath)
	{
		switch (Renderer::GetAPI()) {
		case iRendererAPI::API::NONE:   KRK_ERR("RedererAPI::NONE not supported yet!"); break;
		case iRendererAPI::API::OPENGL: return new OpenGLShader(name, shaderFilePath);
		default:                            KRK_FATAL("Unknown RendererAPI type!");
		}

		return nullptr;
	}
}
