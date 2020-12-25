#pragma once

#include <Testing/TesterBase.hpp>

#ifdef KRAKOA_TEST
namespace krakoa::tests
{
	class MemoryTrackerTester final : public kTest::TesterBase
	{
	public:
		MemoryTrackerTester() noexcept;
		~MemoryTrackerTester() override;

	private:
		void Test() override;
	};
}
#endif