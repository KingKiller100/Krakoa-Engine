#pragma once
#include <Testing/TesterBase.hpp>

#ifdef KRAKOA_TEST
namespace krakoa::tests
{
	class MemPoolerTester final : public kTest::TesterBase
	{
	public:
		MemPoolerTester();
		~MemPoolerTester() override;
		
	private:
		void Test() override;
	public:
		
	};
}
#endif