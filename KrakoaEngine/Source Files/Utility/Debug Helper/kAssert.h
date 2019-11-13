#pragma once

#include <exception>
#include <string>

namespace util
{
	namespace debug
	{
#if _DEBUG

		class AssertOnFailedExpressionException final : public std::exception
		{
		public:
			AssertOnFailedExpressionException(const wchar_t* exp, const wchar_t* msg, const wchar_t* f, const unsigned l);
			~AssertOnFailedExpressionException() throw();

			char const* what() const override;
			virtual void wReportToCharReport();
			
			
		private:
			std::wstring report;
			std::string reportChar;
		};


#define kAssert(condition, msg)\
	{\
		if(condition == false)\
			throw ::util::debug::AssertOnFailedExpressionException(_CRT_WIDE(#condition), _CRT_WIDE(##msg), _CRT_WIDE(__FILE__), (unsigned)(__LINE__));\
	}\

#else
#define kAssert(condition, msg) ((void)0);

#endif

	}
}