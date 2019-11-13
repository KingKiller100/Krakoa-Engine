#pragma once
#include "../../Core/EngineCore.h"
#include "../Calendar/kCalendar.h"

#include <string>
// ***********************************************	UNUSED	**************************************************
namespace util::kLogs
{
	using namespace kCalendar;
	template<class CharT>
	class TemplateLogEntry
	{
		using Message = std::basic_string<CharT>;
		using DateTimeStrType = const char*;
		using FileStrType = const wchar_t*;
		using LineStrType = const unsigned;

	public:
		TemplateLogEntry(Message msg, FileStrType file = L"", LineStrType line = 0)
		: msg(msg), timeStr(GetTimeText().c_str()), dateStr(GetDateInNumericalFormat().c_str()),
		file(file), line(line)
		{}
		
		~TemplateLogEntry()
			= default;

		USE_RESULT constexpr Message GetLogMessage() const
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

