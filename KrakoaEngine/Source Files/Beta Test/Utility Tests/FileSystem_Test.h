#pragma once

#include "../Tester.h"

namespace kTest::utility
{
	class FileSystemTester : public Tester
	{
	public:
		FileSystemTester();
		~FileSystemTester() override;
		
	private:
		void Test() override;
	};
}
