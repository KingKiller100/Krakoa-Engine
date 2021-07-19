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
		void Prepare() noexcept override;
	public:
		void Test();
	};
}
#endif