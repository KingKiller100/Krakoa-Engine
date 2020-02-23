#pragma once

#include <exception>
#include <string>

namespace klib
{
	namespace debug
	{
#if _DEBUG

		class AssertOnFailedConditionException final : public std::exception
		{
		public:
			AssertOnFailedConditionException(const char* exp, const char* msg, const char* f, const unsigned l);
			~AssertOnFailedConditionException() throw();

			char const* what() const override;

		private:
			std::string report;
		};
	}

#define kAssert(condition, msg)\
	{\
		if((condition) == false)\
			throw ::klib::debug::AssertOnFailedConditionException(#condition, ##msg, __FILE__, (unsigned)(__LINE__));\
	}\

#else
	#define kAssert(condition, msg) ((void)0);
#endif

}