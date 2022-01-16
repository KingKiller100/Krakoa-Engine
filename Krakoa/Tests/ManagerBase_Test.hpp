#pragma once

#ifdef KRAKOA_TEST
#	include <Testing/TesterBase.hpp>

namespace krakoa::tests
{
	class ManagerBaseTester final : public kTest::TesterBase
	{
	public:
		ManagerBaseTester() noexcept;
		~ManagerBaseTester() override;

	protected:
		void Prepare() noexcept override;

	private:
		void Test();
	};
}
#endif