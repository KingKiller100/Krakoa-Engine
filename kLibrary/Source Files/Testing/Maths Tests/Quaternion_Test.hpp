﻿#pragma once
#include "../Tester.hpp"


#ifdef TESTING_ENABLED
namespace kTest::Maths
{
	class QuaternionsTester : public Tester
	{
	public:
		QuaternionsTester();
		~QuaternionsTester() override;

	private:
		void Test() override;
	};
}
#endif