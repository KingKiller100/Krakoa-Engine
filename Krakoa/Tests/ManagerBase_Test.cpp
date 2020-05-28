#include "Precompile.hpp"
#include "ManagerBase_Test.hpp"

#include "../Source Files/Patterns/ManagerBase.hpp"

#include <Utility/String/kStringManipulation.hpp>

#ifdef KRAKOA_TEST
namespace krakoa::tests
{
	ManagerBaseTester::ManagerBaseTester() noexcept
		: Tester("ManagerBase Test")
	{}

	ManagerBaseTester::~ManagerBaseTester()
		= default;

	class TestManager : public patterns::ManagerBase<TestManager>
	{
	public:
		TestManager(Token)
			: name("name")
		{}

		~TestManager()
			= default;

	public:
		const std::string name;
	};

	void ManagerBaseTester::Test()
	{
		TestManager::Create();

		const auto& ptr = TestManager::Pointer();
		VERIFY(ptr != nullptr);

		const auto& ref = *ptr;
		VERIFY(ref.name == "name");

		const auto heapDets(TestManager::WalkHeap());
		VERIFY(!heapDets.empty());
		
		constexpr  auto heapPtrTypeSize = sizeof(memory::Heap*);
		constexpr  auto stringTypeSize = sizeof(std::string);
		constexpr  auto managerTypeSize = sizeof(TestManager);
		VERIFY_COMPILE_TIME(managerTypeSize == (heapPtrTypeSize + stringTypeSize));
		
		const auto lines = klib::kString::Split(heapDets, "\n");

		const auto sizeOfTestManagerStr = lines[0];
		const auto dataSizePos= sizeOfTestManagerStr.find(std::to_string(managerTypeSize));
		VERIFY(dataSizePos != std::string::npos);

	}
}
#endif
