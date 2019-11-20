#include "Precompile.h"
#include "Tester.h"

#include <vector>
#include <memory>

namespace kTest
{
	static std::vector<std::unique_ptr<Tester>> kTest_Tests;
	
	Tester::Tester(const char* name) noexcept
		: name(name)
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

	bool Tester::Run()
	{
		try
		{			
			return Test();
		}
		catch (std::exception&)
		{
			return false;
		}
	}
	
}
