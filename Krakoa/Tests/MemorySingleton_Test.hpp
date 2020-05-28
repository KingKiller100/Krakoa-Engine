#pragma once

#include <Testing/Tester.hpp>

namespace krakoa::tests
{
	class MemorySingletonTester final : public kTest::Tester
	{
	public:
		MemorySingletonTester() noexcept;
		~MemorySingletonTester() override;
		
	private:
		void Test() override;		
	};
}
