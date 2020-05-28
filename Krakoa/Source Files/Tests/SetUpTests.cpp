#include "Precompile.hpp"
#include "SetUpTests.hpp"

#include "TestDriver.hpp"

#include "../../Tests/ManagerBase_Test.hpp"

#ifdef KRAKOA_TEST
namespace krakoa::tests
{
	void SetUpTests() noexcept
	{
		TestDriver::AddTest(new ManagerBaseTester());
	}
}
#endif