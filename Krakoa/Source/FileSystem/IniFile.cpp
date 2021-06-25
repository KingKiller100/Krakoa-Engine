#include "Precompile.hpp"

#include "IniFile.hpp"

#include "../Debug/Debug.hpp"
#include "../Debug/Instrumentor.hpp"
#include "../Logging/EngineLogger.hpp"

#include <Utility/FileSystem/kFileSystem.hpp>
#include <Utility/String/kStringTricks.hpp>
#include <Utility/Debug/Source/kMutableSourceInfoToString.hpp>

namespace krakoa::filesystem
{
	IniFile::ValueMap IniFile::ReadFile(const std::filesystem::path& path)
	{
		KRK_PROFILE_FUNCTION();

		ValueMap values;

		auto fileLines = klib::ReadFile(path.string());

		size_t index = 0;

		for (size_t i = 0; i < fileLines.size(); ++i)
		{
			auto& line = fileLines[i];
			klib::MutSourceInfo source(path, index++, "");

			if (const auto commentPos = line.find(s_CommentToken); commentPos != std::string::npos)
				line.erase(commentPos);

			if (klib::IsWhiteSpaceOrNull(line))
				continue;

			klib::Remove(line, ' ');
			const auto colonPos = line.find(':');

			if (colonPos == std::string::npos)
			{
				KRK_FATAL(klib::ToString("[Configurations] Bad line - source: {0}", source));
			}

			const auto key = klib::ToLower(line.substr(0, colonPos));
			const auto value = line.substr(colonPos + 1);

			KRK_LOG("INI", klib::ToString("Mapping: \"{0}\" -> \"{1}\" [{2:f}][{2:l}]", key, value, source));

			values.emplace(key, ValueData{ value, source });
		}

		return values;
	}

	void IniFile::WriteFile(const std::filesystem::path& path, const ValueMap& vMap)
	{
		KRK_LOG("INI", "Writing config file: " + path.string());

		std::string contents;

		for (auto&& data : vMap)
		{
			const auto& key = data.first;
			const auto& value = data.second.value;
			const auto line = util::Fmt("{0}: {1}", key, value);
			contents.append(line + "\n");
			KRK_LOG("INI", line);
		}
		
		klib::WriteFile(path, contents);
	}
}
