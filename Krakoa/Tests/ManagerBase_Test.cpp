#include "Precompile.hpp"
#include "ManagerBase_Test.hpp"

#include "../Source/Patterns/ManagerBase.hpp"
#include "../Source/Memory/MemoryStructures/MemoryTypeSizes.hpp"

#include <Utility/String/kStringTricks.hpp>

#ifdef KRAKOA_TEST
namespace krakoa::tests
{
	ManagerBaseTester::ManagerBaseTester() noexcept
		: TesterBase("ManagerBase Test")
	{}

	ManagerBaseTester::~ManagerBaseTester()
		= default;

	void ManagerBaseTester::Prepare() noexcept
	{
		ADD_TEST(Test());
	}

	class ManagerTest : public patterns::ManagerBase<ManagerTest>
	{
	public:
		ManagerTest(Token)
			: name("name")
		{}

		~ManagerTest()
			= default;

	public:
		const std::string name;
	};

	
	
	void ManagerBaseTester::Test()
	{
		constexpr auto stringTypeSize = sizeof(std::string);
		constexpr auto ptrTypeSize = sizeof(void*);
		
		constexpr auto managerTypeSize = sizeof(ManagerTest);
		
		constexpr auto npos = std::string::npos;
		
		ManagerTest::Create();

		const auto& ptr = ManagerTest::Pointer();
		VERIFY(ptr != nullptr);

		const auto& ref = *ptr;
		VERIFY(ref.name == "name");
		VERIFY_COMPILE_TIME(managerTypeSize == (ptrTypeSize + stringTypeSize));

		const auto status(ManagerTest::GetHeapStatus());
		VERIFY(!status.empty());
		
		const auto lines = klib::kString::Split(status, "\n");

		const auto name = lines[0];
		VERIFY(name.find(typeid(ManagerTest).name()) != npos);

		const auto countStr = lines[1];
		VERIFY(countStr.find("1") != npos);
		
		const auto sizeOfTestManagerStr = lines[2];
		VERIFY(sizeOfTestManagerStr.find(std::to_string(managerTypeSize)) != npos);
		
		const auto sizeOfBlockStr = lines[3];
		VERIFY(sizeOfBlockStr.find(std::to_string(managerTypeSize + memory::ControlBlockSize)) != npos);
		
		const auto totalSizeOFObjectMemoryStr = lines[4];
		VERIFY(totalSizeOFObjectMemoryStr.find(std::to_string(managerTypeSize)) != npos);
		
		const auto totalSizeOFBlockMemoryStr = lines[5];
		VERIFY(totalSizeOFBlockMemoryStr.find(std::to_string(managerTypeSize + memory::ControlBlockSize)) != npos);
	}
}
#endif
