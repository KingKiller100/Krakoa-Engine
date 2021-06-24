#pragma once

#include "../EnvironmentVariables/iEnvironmentVariables.hpp"
#include <unordered_map>

namespace krakoa::os
{
	class EnvironmentVariablesWindows : public iEnvironmentVariables<char>
	{
	public:
		using CharT = char;
		using String = std::basic_string<CharT>;
		using StringView = std::basic_string_view<CharT>;
	
	private:
		static constexpr auto Terminator = klib::type_trait::g_NullTerminator<CharT>;
		static constexpr auto Token = static_cast<CharT>('=');
		
	public:
		EnvironmentVariablesWindows();
		~EnvironmentVariablesWindows() override = default;
		[[nodiscard]] String GetVariable(const String& varKey) const override;
		[[nodiscard]] std::vector<StringView> GetKeys() const override;
		
	
	private:
		void ReadInVariables();

	private:
		std::unordered_map<String, String> variables;
	};
}
