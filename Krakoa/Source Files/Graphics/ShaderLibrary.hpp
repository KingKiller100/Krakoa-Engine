#pragma once

#include "../Patterns/ManagerBase.hpp"

#include <unordered_map>
#include <string>
#include <memory>

namespace krakoa::graphics
{
	class iShader;

	class ShaderLibrary : public patterns::ManagerBase<ShaderLibrary>
	{
	public:
		ShaderLibrary(Token&&);
		~ShaderLibrary();

		USE_RESULT std::weak_ptr<iShader> Get(const std::string& name) const;
		std::weak_ptr<iShader> Load(const std::string_view& filepath);
		std::weak_ptr<iShader> Load(const std::string& name, const std::string_view& filepath);

		bool Exists(const std::string& name) const;
		bool Delete(const std::string& name);
		 
	private:
		std::shared_ptr<iShader> Add(const std::string& name, iShader* shader);

	private:
		std::unordered_map<std::string, std::shared_ptr<iShader>> shadersUMap;
};

}