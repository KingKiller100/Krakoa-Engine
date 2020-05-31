#pragma once

#include <Testing/Tester.hpp>

#ifdef KRAKOA_TEST
namespace krakoa::tests
{
	class MemoryTrackerTester final : public kTest::Tester
	{
	public:
		MemoryTrackerTester() noexcept;
		~MemoryTrackerTester() override;

	private:
		void Test() override;
	};
}
#endif