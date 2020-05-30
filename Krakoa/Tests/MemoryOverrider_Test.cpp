#include "Precompile.hpp"
#include "MemoryOverrider_Test.hpp"

#include "../Source Files/PointerTypes.hpp"
#include "../Source Files/Patterns/MemoryOverrider.hpp"
#include "../Source Files/Core/Logging/MemoryLogger.hpp"
#include "../Source Files/Memory/Memory Structures/MemoryTypes.hpp"

#include <Utility/String/kStringManipulation.hpp>


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
	public:
		friend class MemoryOverriderTester;
		
		~TestMemType()
		{
			if (func)
				func(tester);
		}

		void TestDestructor(const MemoryOverriderTester* test, std::function<void(const MemoryOverriderTester*)> fn)
		{
			tester = test;
			func = std::move(fn);
		}

		std::function<void(const MemoryOverriderTester*)> func;
		const MemoryOverriderTester* tester;
	};

	void MemoryOverriderTester::Test()
	{
		constexpr auto npos = std::string::npos;
		constexpr auto testMemTypeSize = sizeof(TestMemType);
		
		MEM_INIT_LOGS();

		for (auto i = 0; i < 5; ++i)
			const auto* const lifeTime = new TestMemType();

		for (auto i = 0; i < 5; ++i)
			delete new TestMemType();
		
		const auto scope = Make_Solo<TestMemType>();


		const auto status = TestMemType::GetHeapStatus();
		const auto lines = klib::kString::Split(status, '\n');

		const auto name = lines[0];
		VERIFY(name.find(typeid(TestMemType).name()) != npos);

		const auto countStr = lines[1];
		VERIFY(countStr.find("6") != npos);

		const auto sizeOfTestManagerStr = lines[2];
		VERIFY(sizeOfTestManagerStr.find(std::to_string(testMemTypeSize)) != npos);

		const auto sizeOfBlockStr = lines[3];
		VERIFY(sizeOfBlockStr.find(std::to_string(testMemTypeSize + memory::MemoryPaddingBytes)) != npos);

		const auto totalSizeOFObjectMemoryStr = lines[4];
		VERIFY(totalSizeOFObjectMemoryStr.find(std::to_string(testMemTypeSize * 6)) != npos);

		const auto totalSizeOFBlockMemoryStr = lines[5];
		VERIFY(totalSizeOFBlockMemoryStr.find(std::to_string((testMemTypeSize + memory::MemoryPaddingBytes) * 6)) != npos);

		success = false;
		VERIFY(!success);

		scope->TestDestructor(this, [&](const MemoryOverriderTester* ptr)
		{
			VERIFY(ptr == this);
			success = true;
		});
	}
}
#endif