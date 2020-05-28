#include "Precompile.hpp"
#include "SetUpTests.hpp"

#include "TestDriver.hpp"

#include "../../Tests/MemorySingleton_Test.hpp"

namespace krakoa::tests
{
	void SetUpTests() noexcept
	{
		TestDriver::AddTest(new MemorySingletonTester());
	}
}
