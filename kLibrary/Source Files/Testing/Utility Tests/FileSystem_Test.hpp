#pragma once

#include <Testing/Tester.hpp>

#ifdef TESTING_ENABLED
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
#endif
