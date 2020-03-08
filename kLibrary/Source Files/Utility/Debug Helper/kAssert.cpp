#include <pch.hpp>
#include <Utility/Debug Helper/kAssert.hpp>

#include <Utility/Format/kFormatToString.hpp>
#include <Utility/Logging/kLogging_Class.hpp>

namespace klib::debug
{
#if _DEBUG
	AssertOnFailedConditionException::AssertOnFailedConditionException(const char* exp, const char* msg, const char* file, const unsigned line)
		: report(kFormat::ToString("Condition \"%s\" was not met! \n\t\t [NOTE]:   %s.", exp, msg))
	{
		const auto exceptionLog = std::make_unique<kLogs::Logging>();
		exceptionLog->ChangeFilename("Assert Condition Failed");
		exceptionLog->SetMinimumLoggingLevel(kLogs::LLevel::FATL);
		exceptionLog->OutputToFatalFile(report, file, line);
	}

	AssertOnFailedConditionException::~AssertOnFailedConditionException() throw()
	{}
	
	char const* AssertOnFailedConditionException::what() const
	{
		return report.c_str();
	}
#endif
}
