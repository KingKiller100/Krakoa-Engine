#include <pch.hpp>
#include <Utility/Debug Helper/kAssert.hpp>

#include <Utility/Format/kFormatToString.hpp>
#include <Utility/Logging/kLogging.hpp>

namespace klib::debug
{
#if _DEBUG
	AssertOnFailedExpressionException::AssertOnFailedExpressionException(const char* exp, const char* msg, const char* file, const unsigned line)
		: std::exception(),
		report(kFormat::ToString("Expression \"%s\" was not met! \nMessage: %s. \nFile: %s \nLine: %d", exp, msg, file, line))
	{
		INIT_LOGS();
		kLogs::GetMainLogger()->OutputToFatalFile( report, file, line);
	}

	AssertOnFailedExpressionException::~AssertOnFailedExpressionException() throw()
	{
	}


	char const* AssertOnFailedExpressionException::what() const
	{
		return report.c_str();
	}
#endif
}