#include "Precompile.hpp"
#include "ShaderLibrary.hpp"

#include "Resources/iShader.hpp"

#include "iRendererAPI.hpp"
#include "../Debug/Debug.hpp"
#include "../Debug/Instrumentor.hpp"
#include "../FileSystem/VirtualFileExplorer.hpp"

#include <Utility/String/kToString.hpp>
#include <Utility/String/kStringTricks.hpp>

namespace krakoa::gfx
{
	ShaderLibrary::ShaderLibrary()
	{}

	ShaderLibrary::~ShaderLibrary()
		= default;

	void ShaderLibrary::Initialize() const
	{
		const auto rendererAPI = iRendererAPI::GetAPI();
		const auto relativePath = klib::ToString("Keditor\\Assets\\Shaders\\{0}", rendererAPI);
		filesystem::VirtualFileExplorer::Mount(relativePath, "Shaders");
	}

	USE_RESULT std::weak_ptr<iShader> ShaderLibrary::Get(const std::string & name) const
	{
		KRK_PROFILE_FUNCTION();
		KRK_ASSERT(Exists(name), klib::kString::ToString("Trying to retrieve shader that does not currently exist:\n{0}", name));
		return shadersUMap.at(name);
	}

	std::weak_ptr<iShader> ShaderLibrary::Load(const std::string_view& filepath)
	{
		KRK_PROFILE_FUNCTION();
		static auto no_name_shader_count = 0ull;
		const auto& name = klib::kString::ToString("Shader {0}", no_name_shader_count++);
		const auto shader = iShader::Create(name, filepath);
		return Add(name, shader);
	}

	std::weak_ptr<iShader> ShaderLibrary::Load(const std::string& name, const std::string_view& filepath)
	{
		KRK_PROFILE_FUNCTION();
		const auto path = filesystem::VirtualFileExplorer::GetRealPath("Shaders");
		const auto shader = iShader::Create(name, path / filepath);
		return Add(name, shader);
	}

	bool ShaderLibrary::Exists(const std::string& name) const
	{
		KRK_PROFILE_FUNCTION();
		return shadersUMap.find(name) != shadersUMap.end();
	}

	bool ShaderLibrary::Delete(const std::string& name)
	{
		KRK_PROFILE_FUNCTION();
		const auto pathName = klib::kString::Replace(name, '/', '\\');
		if (Exists(pathName))
		{
			shadersUMap.erase(pathName);
			return true;
		}
		return false;
	}

	std::shared_ptr<iShader> ShaderLibrary::Add(const std::string& name, iShader* shader)
	{
		KRK_PROFILE_FUNCTION();
		KRK_ASSERT(!Exists(name)
			, klib::kString::ToString("Adding shader that already exists in the library:\n{0}", name));
		const auto shaderS_Ptr = std::shared_ptr<iShader>(shader);
		shadersUMap.insert(std::make_pair(name, shaderS_Ptr));
		return shaderS_Ptr;
	}

}
