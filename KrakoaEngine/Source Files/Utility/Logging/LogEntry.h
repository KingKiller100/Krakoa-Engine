#pragma once
#include "../../Core/EngineCore.h"

namespace util::kLogs
{
	template<class CharT>
	class TemplateLogEntry
	{
		using Message = const CharT*;
		using TimeStrType = const char*;
		using DateStrType = const char*;
		using InfoType = const wchar_t*;

	public:
		TemplateLogEntry(Message msg, TimeStrType time, DateStrType date, InfoType file = nullptr, InfoType line = nullptr)
		: msg(msg), timeStr(time), dateStr(date),
		file(file), line(line)
		{}
		
		~TemplateLogEntry()
			= default;

		USE_RESULT constexpr Message GetMessage() const
		{
			return msg;
		}
		
		USE_RESULT constexpr TimeStrType GetTime() const
		{
			auto f = __FILE__;
			return timeStr;
		}
		
		USE_RESULT constexpr InfoType GetFile() const
		{
			return file;
		}
		
		USE_RESULT constexpr InfoType GetFileLine() const
		{
			return line;
		}
		
	private:
		Message msg;
		TimeStrType timeStr;
		DateStrType dateStr;
		InfoType file;
		InfoType line;
	};

}

