#pragma once

#ifdef KRAKOA_TEST
#	include <Testing/TesterBase.hpp>

namespace krakoa::tests
{
	class MemoryTrackerTester final : public kTest::TesterBase
	{
	public:
		MemoryTrackerTester() noexcept;
		~MemoryTrackerTester() override;

	private:
		void Prepare() noexcept override;

		void Test();
	};
}
#endif