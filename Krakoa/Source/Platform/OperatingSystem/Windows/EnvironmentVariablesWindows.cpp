#include "Precompile.hpp"
#include "EnvironmentVariablesWindows.hpp"

#include <Utility/String/Tricks/kStringFind.hpp>

#include <Windows.h>

#ifdef GetEnvironmentStrings
#	undef GetEnvironmentStrings
#endif

namespace krakoa::os
{
	EnvironmentVariablesWindows::EnvironmentVariablesWindows()
	{
		ReadInVariables();
	}

	std::basic_string<char> EnvironmentVariablesWindows::GetVariable(const std::basic_string<char>& varKey) const
	{
		return variables.at(varKey);
	}

	std::vector<EnvironmentVariablesWindows::StringView> EnvironmentVariablesWindows::GetKeys() const
	{
		std::vector<StringView> keys;

		for (auto&& variable : variables)
		{
			keys.emplace_back(variable.first);
		}

		return keys;
	}

	void EnvironmentVariablesWindows::ReadInVariables()
	{
		auto* envStr = GetEnvironmentStrings();

		while (*envStr != '\0')
		{
			++envStr;
		}

		++envStr;

		while (*envStr != '\0')
		{
			if (*envStr == '=')
			{}
			else
			{
				const auto eqPos = klib::Find_First_Of(envStr, '=');
				if (eqPos != Terminator)
				{
					String key(envStr, eqPos);
					envStr += eqPos + 1;
					String value = envStr;
					envStr += value.size();
					variables.emplace(klib::ToLower(key), value);
				}
			}
			
			++envStr;
		}
	}
}
