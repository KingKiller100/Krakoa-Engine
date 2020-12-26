#include "Precompile.hpp"
#include "iShader.hpp"

#include "../Renderer.hpp"

#include "../../Debug/Debug.hpp"
#include "../../Core/Logging/CoreLogger.hpp"
#include "../../Platform/OpenGL/OpenGLShader.hpp"

#include <Utility/FileSystem/kFileSystem.hpp>


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
		case iRendererAPI::ApiType::NONE:   KRK_ERRR("RedererAPI::NONE not supported yet!"); break;
		case iRendererAPI::ApiType::OPENGL: return new OpenGLShader(name, path);
		default:                            KRK_FATAL("Unknown RendererAPI type!");
		}

		return nullptr;
	}
}
