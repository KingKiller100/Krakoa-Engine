#include "Precompile.hpp"
#include "MemoryOverrider_Test.hpp"

#include "../Source Files/Memory/PointerTypes.hpp"
#include "../Source Files/Patterns/MemoryOverrider.hpp"

#ifdef KRAKOA_TEST
namespace krakoa::tests
{
	MemoryOverriderTester::MemoryOverriderTester() noexcept
		: Tester("Memory Overrider Test")
	{}

	MemoryOverriderTester::~MemoryOverriderTester()
		= default;

	class TestMemType : public patterns::MemoryOverrider<TestMemType>
	{
		
	};

	void MemoryOverriderTester::Test()
	{
		const auto* const lifeTime = new TestMemType();
		VERIFY(lifeTime != nullptr);

		const auto scope = Make_Solo<TestMemType>();
		VERIFY(scope != nullptr);
		
	}
}
#endif