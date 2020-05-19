#pragma once

#include "../../../HelperMacros.hpp"
#include "../../../Maths/Length_Type.hpp"

#include <stdexcept>


namespace klib::kDebug::errors
{
	class DivByZero final : public std::logic_error
	{
	public:
		USE_RESULT explicit DivByZero();
		USE_RESULT explicit DivByZero(const std::string& _Message);

		USE_RESULT explicit DivByZero(const char* const _Message);

		~DivByZero() noexcept override = default;
	};

	class NoRealRoot final : public std::logic_error
	{
	public:
		USE_RESULT explicit NoRealRoot(const long double value);

		USE_RESULT explicit NoRealRoot(const double value);
		
		USE_RESULT explicit NoRealRoot(const float value);
		
		USE_RESULT explicit NoRealRoot(const kmaths::Big_Int_Type value);

		USE_RESULT explicit NoRealRoot(const std::string& _Message);

		USE_RESULT explicit NoRealRoot(const char* const _Message);

		~NoRealRoot() noexcept override;
	};

	class BreaksMathsLogic final : public std::logic_error
	{
	public:
		USE_RESULT explicit BreaksMathsLogic(const std::string& _Message);
		USE_RESULT explicit BreaksMathsLogic(const char* const _Message);
		~BreaksMathsLogic() noexcept override;
	};

}
