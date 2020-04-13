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

	iShader * iShader::Create(const std::string_view & vertexSourceFilePath, const std::string_view & fragmentSourceFilePath)
	{
		const auto currentDir = klib::kFileSystem::GetExeDirectory();

		const auto formattedVSPath = klib::kString::Replace(vertexSourceFilePath, '/', '\\');
		const auto formattedFSPath = klib::kString::Replace(fragmentSourceFilePath, '/', '\\');

		const auto vsPath = currentDir + formattedVSPath.data();
		const auto fsPath = currentDir + formattedFSPath.data();

		const auto vsData = klib::kFileSystem::ParseFileData(vsPath);
		const auto fsData = klib::kFileSystem::ParseFileData(fsPath);

		KRK_FATAL(!vsData.empty(), "Vertex Shader data empty");
		KRK_FATAL(!fsData.empty(), "Fragment Shader data empty");

		std::string vs;
		std::string fs;

		for (auto& line : vsData)
			vs.append(line + '\n');

		for (auto& line : fsData)
			fs.append(line + '\n');

		switch (Renderer::Reference().GetAPI()) {
		case iRendererAPI::API::NONE:   KRK_FATAL(false, "RedererAPI::NONE not supported yet!"); break;
		case iRendererAPI::API::OPENGL: return new OpenGLShader(vs, fs);
		default:                        KRK_FATAL(false, "Unknown RendererAPI type!");
		}

		return nullptr;
	}
}