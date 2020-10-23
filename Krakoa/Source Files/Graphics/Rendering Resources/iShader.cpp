#include "Precompile.hpp"
#include "iShader.hpp"

#include "../Renderer.hpp"

#include "../../Core/Logging/CoreLogger.hpp"
#include "../../Platform/OpenGL/OpenGLShader.hpp"

#include <Utility/File System/kFileSystem.hpp>

namespace krakoa::graphics
{
	iShader::~iShader()
		= default;

	iShader * iShader::Create(const std::string_view& name, const std::string_view & shaderFilePath)
	{
		const std::string currentDir(klib::kFileSystem::GetExeDirectory());

		const auto formattedPath = klib::kString::Replace(shaderFilePath, '/', '\\');

		const auto path = currentDir + formattedPath.data();

		switch (Renderer::GetAPI()) {
		case iRendererAPI::API::NONE:   KRK_FATAL(false, "RedererAPI::NONE not supported yet!"); break;
		case iRendererAPI::API::OPENGL: return new OpenGLShader(name, path);
		default:                        KRK_FATAL(false, "Unknown RendererAPI type!");
		}

		return nullptr;
	}
}