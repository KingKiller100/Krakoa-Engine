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
		klib::kDebug::CheckRemoteDebuggerAttached("waitforremotedebugger");
		//klib::kDebug::BreakPointHere(); // Works Great!
		//klib::kAssert(false, "Working Great!"); 
	}
}
#endif
