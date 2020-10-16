#pragma once

#include <array>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <string_view>
#include "../../Maths/kAlgorithms.hpp"

namespace klib
{
	namespace kEnums
	{

#define ENUM_CLASS(name, base_inherit, ...)\
		struct name\
		{\
		public:\
			enum _enum base_inherit { __VA_ARGS__ };\
			inline static constexpr auto count = kmaths::Count( __VA_ARGS__ );\
\
			inline static constexpr std::array<_enum, count> arr_enum = { __VA_ARGS__ };\
			inline static const std::string raw_names = #__VA_ARGS__;\
\
			static std::string_view ToString(const _enum e)\
			{\
				static bool first_entry = true;\
				static auto mapper = std::unordered_map<_enum, std::string_view>();\
\
				if (first_entry)\
				{\
					const auto names = GetNames();\
					for (auto i = 0ull; i < count; ++i)\
					{\
						mapper.insert(std::make_pair(\
							arr_enum[i]\
							, names[i]));\
					}\
					first_entry = false;\
				}\
\
				return mapper.at(e);\
			}\
			\
		private:\
			static std::vector<std::string> GetNames()\
			{\
				std::vector<std::string> names(count);\
				for (auto commaPos = raw_names.find_first_of(",");\
					commaPos != std::string::npos;)\
					{\
						auto temp = raw_names.substr(0, commaPos);\
						temp.erase(std::remove_if(temp.begin(), temp.end(), std::isspace), temp.end());\
						names.push_back(temp);\
				}\
				return names; \
			}\
		};\
		
	}
}
