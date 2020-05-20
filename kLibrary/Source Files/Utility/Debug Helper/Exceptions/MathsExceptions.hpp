#pragma once

#include "../../../HelperMacros.hpp"
#include "../../../Maths/Length_Type.hpp"

#include <stdexcept>


namespace klib::kDebug::errors
{
	class DivByZero final : public std::logic_error
	{
	public:
		explicit DivByZero();
		explicit DivByZero(const char* const _Message);
		explicit DivByZero(const std::string& _Message);

		~DivByZero() noexcept override = default;
	};

	class NoRealRoot final : public std::logic_error
	{
	public:
		explicit NoRealRoot(const float value);		
		explicit NoRealRoot(const double value);
		explicit NoRealRoot(const long double value);
		explicit NoRealRoot(const kmaths::Big_Int_Type value);

		explicit NoRealRoot(const char* const _Message);
		explicit NoRealRoot(const std::string& _Message);

		~NoRealRoot() noexcept override;
	};

	class BreaksMathsLogic final : public std::logic_error
	{
	public:
		explicit BreaksMathsLogic(const char* const _Message);
		explicit BreaksMathsLogic(const std::string& _Message);

		~BreaksMathsLogic() noexcept override;
	};

}
