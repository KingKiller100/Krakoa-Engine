#include "Precompile.hpp"
#include "ShaderLibrary.hpp"

#include "Rendering Resources/iShader.hpp"
#include "../Core/Logging/CoreLogger.hpp"

#include <Utility/Format/kFormatToString.hpp>
#include <Utility/String/kStringManipulation.hpp>

namespace krakoa::graphics
{
	ShaderLibrary::ShaderLibrary(Token &&)
	{}

	ShaderLibrary::~ShaderLibrary()
		= default;

	USE_RESULT iShader & ShaderLibrary::Get(const std::string & name) const
	{
		KRK_FATAL(Exists(name), klib::kFormat::ToString("Trying to retrieve shader that does not currently exist:\n%s", name.data()));
		return *shadersUMap.at(name);
	}

	krakoa::graphics::iShader& ShaderLibrary::Load(const std::string_view& filepath)
	{
		static auto no_name_shader_count = 0ull;
		const auto& name = klib::kFormat::ToString("Shader %u", no_name_shader_count++);
		const auto shader = iShader::Create(name, filepath);
		Add(name, shader);
		return *shader;
	}

	krakoa::graphics::iShader& ShaderLibrary::Load(const std::string& name, const std::string_view& filepath)
	{
		const auto shader = iShader::Create(name, filepath);
		Add(name, shader);
		return *shader;
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		return shadersUMap.find(name) != shadersUMap.end();
	}

	bool ShaderLibrary::Delete(const std::string& name)
	{
		const auto pathName = klib::kString::Replace(name, '/', '\\');
		if (Exists(pathName))
		{
			shadersUMap.erase(pathName);
			return true;
		}
		return false;
	}

	void ShaderLibrary::Add(const std::string& name, iShader* shader)
	{
		KRK_FATAL(!Exists(name), klib::kFormat::ToString("Trying to add shader to library that already exists:\n%s", name.data()));
		shadersUMap.insert(std::make_pair(name, shader));
	}

}
