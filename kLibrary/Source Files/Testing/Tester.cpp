#include <pch.hpp>
#include <Testing/Tester.hpp>

#ifdef TESTING_ENABLED

namespace kTest
{
	Tester::Tester(const char* name) noexcept
		: name(name), success(true)
	{	}

	Tester::Tester(Tester&& other) noexcept
		: success(true)
	{
		*this = std::move(other);
	}

	Tester& Tester::operator=(Tester&& other) noexcept
	{
		this->name = std::move(other.name);
		this->result = std::move(other.result);
		return *this;
	}

	Tester::~Tester()
		= default;
	   
	const char* Tester::GetName() const noexcept
	{
		return name.c_str();
	}

	std::string Tester::GetFailureData() const noexcept
	{
		return result;
	}

	bool Tester::Run() noexcept
	{
		try
		{
			Test();
			return success;
		}
		catch (std::exception&)
		{
			return false;
		}
	}
}

#endif