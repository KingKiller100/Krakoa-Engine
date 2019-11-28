#include <Precompile.h>
#include <Utility/Debug Helper/kAssert.hpp>

#include <Utility/Format/kFormatToString.hpp>
#include <Utility/Logging/kLogging.hpp>

namespace util::debug
{
	AssertOnFailedExpressionException::AssertOnFailedExpressionException(const wchar_t* exp, const wchar_t* msg, const wchar_t* file, const unsigned line)
		: std::exception(),
		report(kFormat::FormatToString(L"Expression \"%s\" was not met! \nMessage: %s. \nFile: %s \nLine: %d", exp, msg, file, line))
	{
		wReportToCharReport();

		char fileBuffer[512];
		const auto fileStr = std::wstring_view(file);
		auto fileLength = fileStr.size();
		wcstombs_s(&fileLength, fileBuffer, fileStr.data(), sizeof (fileBuffer));
		
		INIT_LOGS();
		kLogs::GetLogger()->OutputToFatalFile( reportChar, fileBuffer, line);
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
