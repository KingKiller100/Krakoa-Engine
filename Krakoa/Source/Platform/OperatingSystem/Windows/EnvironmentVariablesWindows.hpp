#pragma once

#include "../EnvironmentVariables/iEnvironmentVariables.hpp"
#include <unordered_map>

#if defined(_WIN32) || defined(KRAKOA_OS_WINDOWS)

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
		[[nodiscard]] std::basic_string_view<CharT> GetVariable( const std::basic_string_view<CharT>& varKey ) const override;
		[[nodiscard]] std::vector<StringView> GetKeys() const override;
		
	
	private:
		void ReadInVariables();
		String ResolveKey(StringView key) const;

	private:
		std::unordered_map<String, String> variables;
	};
}

#endif

