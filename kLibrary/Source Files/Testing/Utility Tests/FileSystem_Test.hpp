#pragma once

#include <Testing/Tester.hpp>

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
