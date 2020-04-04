#pragma once
#include "../Tester.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	class TimerTester : public Tester
	{
	public:
		TimerTester();
		~TimerTester() override;
		
	private:
		void Test() override;
	};
}
#endif
