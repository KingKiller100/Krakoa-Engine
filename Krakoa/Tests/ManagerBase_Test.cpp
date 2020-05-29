#include "Precompile.hpp"
#include "ManagerBase_Test.hpp"

#include "../Source Files/Patterns/ManagerBase.hpp"
#include "../Source Files/Memory/Memory Structures/MemoryTypes.hpp"

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
		constexpr auto stringTypeSize = sizeof(std::string);
		constexpr auto ptrTypeSize = sizeof(void*);
		
		constexpr auto managerTypeSize = sizeof(TestManager);
		
		constexpr auto npos = std::string::npos;
		
		TestManager::Create();

		const auto& ptr = TestManager::Pointer();
		VERIFY(ptr != nullptr);

		const auto& ref = *ptr;
		VERIFY(ref.name == "name");
		VERIFY_COMPILE_TIME(managerTypeSize == (ptrTypeSize + stringTypeSize));

		const auto status(TestManager::GetHeapStatus());
		VERIFY(!status.empty());
		
		const auto lines = klib::kString::Split(status, "\n");

		const auto name = lines[0];
		VERIFY(name.find(typeid(TestManager).name()) != npos);

		const auto countStr = lines[1];
		VERIFY(countStr.find("1") != npos);
		
		const auto sizeOfTestManagerStr = lines[2];
		VERIFY(sizeOfTestManagerStr.find(std::to_string(managerTypeSize)) != npos);
		
		const auto sizeOfBlockStr = lines[3];
		VERIFY(sizeOfBlockStr.find(std::to_string(managerTypeSize + memory::MemoryPaddingBytes)) != npos);
		
		const auto totalSizeOFObjectMemoryStr = lines[4];
		VERIFY(totalSizeOFObjectMemoryStr.find(std::to_string(managerTypeSize)) != npos);
		
		const auto totalSizeOFBlockMemoryStr = lines[5];
		VERIFY(totalSizeOFBlockMemoryStr.find(std::to_string(managerTypeSize + memory::MemoryPaddingBytes)) != npos);
	}
}
#endif
