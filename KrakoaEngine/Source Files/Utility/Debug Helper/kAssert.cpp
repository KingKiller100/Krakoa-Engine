#include "Precompile.h"
#include "kAssert.h"

#include "../Format To String/kFormatToString.h"
#include "../Logging/kLog_Class.h"

namespace util::debug
{
	AssertOnFailedExpressionException::AssertOnFailedExpressionException(const wchar_t* exp, const wchar_t* msg, const wchar_t* f, const unsigned l)
		: std::exception(),
		report(kFormatToString::FormatToString(L"Expression \"%s\" was not met! \nMessage: %s. \nFile: %s \nLine: %d", exp, msg, f, l))
	{
		wReportToCharReport();
		kLogs::GetLogger()->InitializeLogging();
		kLogs::GetLogger()->OutputToFatalFile( reportChar );
	}

	AssertOnFailedExpressionException::~AssertOnFailedExpressionException() throw()
	{
	}


	char const* AssertOnFailedExpressionException::what() const
	{
		return reportChar.c_str();
	}

	void AssertOnFailedExpressionException::wReportToCharReport()
	{
		char tempChar[1024];
		auto count = size_t(0);
		auto currentChar = report.c_str();

		while (*currentChar != (L'\0'))
		{
			++currentChar;
			++count;
		}
		wcstombs_s(&count, tempChar, report.c_str(), sizeof(tempChar));

		reportChar = tempChar;
	}
}
