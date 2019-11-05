#pragma once

#include <exception>
#include <string>

namespace util
{
	namespace debug
	{
#if _DEBUG

		class AssertOnFailedExpression final : public std::exception
		{
		public:
			AssertOnFailedExpression(const wchar_t* exp, const wchar_t* msg, const wchar_t* f, const unsigned l);
			~AssertOnFailedExpression() throw();

			char const* what() const override;

		private:
			const wchar_t* expression;
			const wchar_t* message;
			const wchar_t* file;
			const unsigned line;
			std::wstring report;
		};


#define kAssert(condition, msg)\
	{\
		if(condition == false)\
			throw ::util::debug::AssertOnFailedExpression(_CRT_WIDE(#condition), _CRT_WIDE(#msg), _CRT_WIDE(__FILE__), (unsigned)(__LINE__));\
	}\

#else
#define kAssert(condition, msg) ((void)0);

#endif

	}
}