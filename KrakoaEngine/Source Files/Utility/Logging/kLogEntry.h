#pragma once
#include "../../Core/EngineCore.h"

#include <type_traits>
#include <string>

namespace util::kLogs
{
	template<class CharT>
	class TemplateLogEntry
	{
		using Message = std::basic_string_view<CharT>;
		using DateTimeStrType = const char*;
		using FileStrType = const wchar_t*;
		using LineStrType = const unsigned;

	public:
		TemplateLogEntry(Message msg, DateTimeStrType time, DateTimeStrType date, FileStrType file = nullptr, LineStrType line = 0)
		: msg(msg), timeStr(time), dateStr(date),
		file(file), line(line)
		{}
		
		~TemplateLogEntry()
			= default;

		USE_RESULT constexpr Message GetMessage() const
		{
			return msg;
		}
		
		USE_RESULT constexpr DateTimeStrType GetTime() const
		{
			return timeStr;
		}

		USE_RESULT constexpr DateTimeStrType GetDate() const
		{
			return dateStr;
		}
		
		USE_RESULT constexpr FileStrType GetFile() const
		{
			return file;
		}
		
		USE_RESULT constexpr LineStrType GetFileLine() const
		{
			return line;
		}
		
	private:
		Message msg;
		DateTimeStrType timeStr;
		DateTimeStrType dateStr;
		FileStrType file;
		LineStrType line;
	};

}

