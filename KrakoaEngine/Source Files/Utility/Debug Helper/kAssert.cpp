#include "Precompile.h"
#include "kAssert.h"

#include "../Format/kFormatToString.h"
#include "../Logging/kLogging.h"

namespace util::debug
{
	AssertOnFailedExpressionException::AssertOnFailedExpressionException(const wchar_t* exp, const wchar_t* msg, const wchar_t* f, const unsigned l)
		: std::exception(),
		report(kFormatToString::FormatToString(L"Expression \"%s\" was not met! \nMessage: %s. \nFile: %s \nLine: %d", exp, msg, f, l))
	{
		wReportToCharReport();

		char fileBuffer[512];
		const auto fileStr = std::wstring_view(f);
		auto fileLength = fileStr.size();
		wcstombs_s(&fileLength, fileBuffer, fileStr.data(), sizeof (fileBuffer));
		
		kLogs::GetLogger()->InitializeLogging();
		kLogs::GetLogger()->OutputToFatalFile( reportChar, fileBuffer, l);
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
		auto count = report.size();
		
		wcstombs_s(&count, tempChar, report.c_str(), sizeof(tempChar));

		reportChar = tempChar;
	}
}
