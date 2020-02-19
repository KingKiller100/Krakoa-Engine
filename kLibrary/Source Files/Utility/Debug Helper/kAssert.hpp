#pragma once

#include <exception>
#include <string>

namespace klib
{
	namespace debug
	{
#if _DEBUG

		class AssertOnFailedExpressionException final : public std::exception
		{
		public:
			AssertOnFailedExpressionException(const char* exp, const char* msg, const char* f, const unsigned l);
			~AssertOnFailedExpressionException() throw();

			char const* what() const override;

		private:
			std::string report;
		};
	}

#define kAssert(condition, msg)\
	{\
		if((condition) == false)\
			throw ::klib::debug::AssertOnFailedExpressionException(#condition, ##msg, __FILE__, (unsigned)(__LINE__));\
	}\

#else
	#define kAssert(condition, msg) ((void)0);
#endif

}