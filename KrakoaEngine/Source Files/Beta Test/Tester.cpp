#include "Precompile.h"
#include "Tester.h"

#include <vector>
#include <memory>

namespace kTest
{
	static std::vector<std::unique_ptr<Tester>> kTest_Tests;
	
	Tester::Tester(const char* name) noexcept
		: name(name), success(true)
	{	
	}

	Tester::Tester(Tester&& other) noexcept
	{
		*this = std::move(other);
	}

	Tester& Tester::operator=(Tester&& other) noexcept
	{
		this->name = std::move(other.name);
		
		return *this;
	}

	Tester::~Tester()
		= default;
	   
	const char* Tester::GetName() const
	{
		return name.c_str();
	}

	std::string Tester::GetResult() const
	{
		return result.str();
	}

	bool Tester::Run()
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
