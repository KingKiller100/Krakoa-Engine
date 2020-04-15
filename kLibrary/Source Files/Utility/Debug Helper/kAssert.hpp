#pragma once

#include <exception>
#include <string>

namespace klib
{
#if _DEBUG 
	namespace kDebug
	{
		class AssertOnFailedConditionException final : public std::exception
		{
		public:
			AssertOnFailedConditionException(const std::string_view& exp, const std::string_view& msg, const char* f, const unsigned l);
			~AssertOnFailedConditionException() throw();

			char const* what() const override;

		private:
			std::string report;
		};
	}

#	define kAssert(condition, msg)\
	{\
		if((condition) == false)\
			throw ::klib::kDebug::AssertOnFailedConditionException(#condition, ##msg, __FILE__, (unsigned)(__LINE__));\
	}\

#else
#	define kAssert(condition, msg) ((void)0);
#endif
}
