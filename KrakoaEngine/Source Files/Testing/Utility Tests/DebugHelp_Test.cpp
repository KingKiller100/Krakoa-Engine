#include <Precompile.h>
#include <Testing/Utility Tests/DebugHelp_Test.hpp>

#include <Utility/Debug Helper/kDebugger.hpp>
#include <Utility/Debug Helper/kAssert.hpp>

namespace kTest::utility
{	
	DebugHelpTester::DebugHelpTester()
		: Tester("Debug Help Test")
	{
	}

	DebugHelpTester::~DebugHelpTester()
		= default;

	void DebugHelpTester::Test()
{
		util::kDebugger::CheckRemoteDebuggerAttached("waitforremotedebugger");
		//util::kDebugger::BreakPointHere(); // Works Great!
		//util::kAssert(false, "Working Great!"); 
	}


}
