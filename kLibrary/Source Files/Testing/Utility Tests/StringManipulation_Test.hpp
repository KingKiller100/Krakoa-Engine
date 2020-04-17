#pragma once

#include "../Tester.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	class StringManipulationTester : public Tester
	{
	public:
		StringManipulationTester();
		~StringManipulationTester();

	private:
		void Test() override;
	};

}
#endif