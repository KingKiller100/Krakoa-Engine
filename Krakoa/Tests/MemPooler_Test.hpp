#pragma once
#include <Testing/Tester.hpp>

#ifdef KRAKOA_TEST
namespace krakoa::tests
{
	class MemPoolerTester final : public kTest::Tester
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