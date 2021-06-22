#pragma once

#include <Utility/Debug/Source/kMutableSourceInfo.hpp>

#include <filesystem>
#include <string>
#include <unordered_map>

namespace krakoa::filesystem
{
	class IniFile
	{
	public:
		struct ValueData
		{
			std::string value;
			klib::MutSourceInfo source;
		};
		
		using ValueMap = std::unordered_map<std::string, ValueData>;

		static constexpr auto s_CommentToken = '*';
	
	public:
		static ValueMap ReadFile(const std::filesystem::path& path);
		static void WriteFile(const std::filesystem::path& path, const ValueMap& vMap);
	};
}
