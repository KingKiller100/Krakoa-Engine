#include "Precompile.h"
#include "DebugHelp_Test.h"

#include "../../Utility/Debug Helper/kDebugger.h"
#include "../../Utility/Debug Helper/kAssert.h"

namespace kTest::utility
{
	using namespace util;
	
	DebugHelpTester::DebugHelpTester()
		: Tester("Debug Help Test")
	{
	}

	DebugHelpTester::~DebugHelpTester()
		= default;

	void DebugHelpTester::Test()
	{
		kDebugger::CheckRemoteDebuggerAttached("waitforremotedebugger");
		//kDebugger::BreakPointHere(); // Works Great!
		//kAssert(false, "Working Great!"); 
	}


}
