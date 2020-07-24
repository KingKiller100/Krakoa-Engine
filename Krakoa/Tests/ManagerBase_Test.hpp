#pragma once

#include <Testing/Tester.hpp>

#ifdef KRAKOA_TEST
namespace krakoa::tests
{
	class ManagerBaseTester final : public kTest::Tester
	{
	public:
		ManagerBaseTester() noexcept;
		~ManagerBaseTester() override;

	private:
		void Test() override;
	};
}
#endif