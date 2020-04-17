#pragma once

#include "../Patterns/SimpleSingleton.hpp"

#include <unordered_map>
#include <string>
#include <memory>

namespace krakoa::graphics
{
	class iShader;

	class ShaderLibrary : public pattern::SimpleSingleton<ShaderLibrary>
	{
	public:
		ShaderLibrary(Token&&);
		~ShaderLibrary();

		USE_RESULT iShader& Get(const std::string& name) const;
		iShader& Load(const std::string_view& filepath);
		iShader& Load(const std::string& name, const std::string_view& filepath);

		bool Exists(const std::string& name) const;
		bool Delete(const std::string& name);
		 
	private:
		void Add(const std::string& name, iShader* shader);

	private:
		std::unordered_map<std::string, std::unique_ptr<iShader>> shadersUMap;
};

}