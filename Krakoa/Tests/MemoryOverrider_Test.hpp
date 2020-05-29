#pragma once

#include <Testing/Tester.hpp>

#ifdef KRAKOA_TEST
namespace krakoa::tests
{
	class MemoryOverriderTester final : public kTest::Tester
	{
	public:
		MemoryOverriderTester() noexcept;
		~MemoryOverriderTester() override;

	private:
		void Test() override;
	};
}
#endif