#include "Precompile.hpp"
#include "iShader.hpp"

#include "../Renderer.hpp"

#include "../../Debug/Debug.hpp"
#include "../../Logging/EngineLogger.hpp"
#include "../../Platform/OpenGL/OpenGLShader.hpp"

#include <Utility/FileSystem/kFileSystem.hpp>
#include <Utility/String/Tricks/kStringOperators.hpp>

using namespace klib::operators;

namespace krakoa::graphics
{
	iShader::~iShader()
		= default;

	iShader * iShader::Create(const std::string_view& name, const std::string_view & shaderFilePath)
	{
		const auto cwd = klib::GetCurrentWorkingDirectory();
		
		const auto formattedPath = klib::Replace(cwd + shaderFilePath, '/', '\\');

		switch (Renderer::GetAPI()) {
		case iRendererAPI::ApiType::NONE:   KRK_ERR("RedererAPI::NONE not supported yet!"); break;
		case iRendererAPI::ApiType::OPENGL: return new OpenGLShader(name, formattedPath);
		default:                            KRK_FATAL("Unknown RendererAPI type!");
		}

		return nullptr;
	}
}
