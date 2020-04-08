#include "pch.hpp"
#include "kAssert.hpp"

#include "../Format/kFormatToString.hpp"
#include "../Logging/kLogging_Class.hpp"

namespace klib::debug
{
#if _DEBUG
	AssertOnFailedConditionException::AssertOnFailedConditionException(const char* exp, const char* msg, const char* file, const unsigned line)
		: report(kFormat::ToString("Condition \"%s\" was not met! \n               [NOTE]: %s.", exp, msg))
	{
		auto exceptionLog = kLogs::Logging();
		exceptionLog.SetName("kAssert");
		exceptionLog.ChangeFilename("Assert Condition Failed");
		exceptionLog.ToggleSubSystemEnabled();
		exceptionLog.SetMinimumLoggingLevel(kLogs::LLevel::FATL);
		exceptionLog.AddEntry(report, kLogs::LLevel::FATL, file, line);
	}

	AssertOnFailedConditionException::~AssertOnFailedConditionException() throw()
	{}
	
	char const* AssertOnFailedConditionException::what() const
	{
		return report.c_str();
	}
#endif
}
