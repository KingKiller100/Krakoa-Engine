#include "Precompile.h"
#include "TesterManager.h"

#include "Tester.h"

#include "Maths Tests/Vectors_Test.h"
#include "Utility Tests/Calendar_Test.h"
#include "Utility Tests/Clock_Test.h"
#include "Utility Tests/DebugHelp_Test.h"
#include "Utility Tests/FileSystem_Test.h"
#include "Utility Tests/Logging_Test.h"
#include "Utility Tests/StringView_Test.h"

#include <iostream>

namespace kTest
{	
	TesterManager::TesterManager(Token)
	{
	}

	TesterManager::~TesterManager()
		= default;

	void TesterManager::Initialize()
	{		
		Add(new utility::CalendarTester());
		Add(new utility::ClockTester());
		Add(new utility::DebugHelpTester());
		Add(new utility::FileSystemTester());
		Add(new Maths::VectorsTester());
		Add(new utility::StringViewTester());
		Add(new utility::LoggingTester());
	}

	void TesterManager::Add(Tester* test)
	{
		tests.insert(
			std::make_pair(test->GetName(), 
				std::unique_ptr<Tester>(std::move(test))));
	}

	void TesterManager::RunAll()
	{
		for (auto& test : tests)
		{
			const auto result = test.second->Run() ? "Success: " : "Failed: ";
			std::cout << result << test.first << "\n";
		}
	}
}
