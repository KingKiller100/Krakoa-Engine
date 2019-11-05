#include "kAssert.h"

#include "../Format To String/kFormatToString.h"
#include "../Logging/kLog_Class.h"

namespace util::debug
{
	AssertOnFailedExpression::AssertOnFailedExpression(const wchar_t* exp, const wchar_t* msg, const wchar_t* f, const unsigned l)
		: std::exception(),
		expression(exp),
		message(msg),
		file(f),
		line(l),
		report(kFormatToString::FormatToString(L"Expression \"%s\" was not met! \nMessage: \"%s\". \nFile: %s \nLine: %d", expression, message, f, l))
	{
		char tempChar[1024];

		auto count = size_t(0);
		auto currentChar = report.c_str();
		while (*currentChar != wchar_t('\0'))
		{
			++currentChar;
			++count;
		}

		wcstombs_s(&count, tempChar, report.c_str(), sizeof(tempChar));
		
		kLogs::logger->InitializeLogging();
		kLogs::logger->OutputToFatalFile(tempChar);
	}

	AssertOnFailedExpression::~AssertOnFailedExpression() throw()
	{
	}


	char const* AssertOnFailedExpression::what() const
	{
		char tempChar[1024];
		
		auto count = size_t(0);
		auto currentChar = report.c_str();
		while (*currentChar != wchar_t('\0'))
		{
			++currentChar;
			++count;
		}

		wcstombs_s(&count, tempChar, report.c_str(), sizeof(tempChar));

		return std::string(tempChar).c_str();
	}

}
