#pragma once

#include <Testing/TesterBase.hpp>

#ifdef KRAKOA_TEST
namespace krakoa::tests
{
	class ManagerBaseTester final : public kTest::TesterBase
	{
	public:
		ManagerBaseTester() noexcept;
		~ManagerBaseTester() override;

	private:
		void Test() override;
	};
}
#endif