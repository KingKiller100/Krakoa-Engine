#include "pch.hpp"
#include "DebugHelp_Test.hpp"

#include "../../Utility/Debug Helper/kDebugger.hpp"
#include "../../Utility/Debug Helper/kAssert.hpp"

#ifdef TESTING_ENABLED
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
		klib::kDebugger::CheckRemoteDebuggerAttached("waitforremotedebugger");
		//util::kDebugger::BreakPointHere(); // Works Great!
		//util::kAssert(false, "Working Great!"); 
	}
}
#endif
