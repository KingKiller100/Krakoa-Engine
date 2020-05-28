#include "Precompile.hpp"
#include "MemorySingleton_Test.hpp"

#include "../Source Files/Patterns/MemorySingleton.hpp"

namespace krakoa::tests
{
	MemorySingletonTester::MemorySingletonTester() noexcept
		: Tester("Memory Singleton Test")
	{}

	MemorySingletonTester::~MemorySingletonTester()
		= default;

	class TestClass : public patterns::MemorySingleton<TestClass>
	{
	public:
		TestClass(Token)
		{}

		~TestClass()
			= default;

		const std::string name = "name";
	};

	void MemorySingletonTester::Test()
	{
		TestClass::Create();

		const auto& ptr = TestClass::Pointer();
		VERIFY(ptr != nullptr);

		const auto& ref = *ptr;
		VERIFY(ref.name == "name");
	}
}
