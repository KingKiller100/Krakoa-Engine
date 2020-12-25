#include "Precompile.hpp"
#include "MemoryTracker_Test.hpp"

#include "../Source Files/PointerTypes.hpp"
#include "../Source Files/Patterns/MemoryTracker.hpp"
#include "../Source Files/Core/Logging/MemoryLogger.hpp"
#include "../Source Files/Memory/Memory Structures/MemoryTypeSizes.hpp"

#include <Utility/String/kStringTricks.hpp>

#include <functional>

#ifdef KRAKOA_TEST
namespace krakoa::tests
{
	MemoryTrackerTester::MemoryTrackerTester() noexcept
		: TesterBase("Memory Tracker Test")
	{}

	MemoryTrackerTester::~MemoryTrackerTester()
		= default;

	class TestMemType : public patterns::MemoryTracker<TestMemType>
	{
	public:
		friend class MemoryTrackerTester;
		
		TestMemType(const MemoryTrackerTester* test)
			: tester(test)
		{}
		
		~TestMemType()
		{
			if (func && tester)
				func(tester);
		}

		void SetDestructorCallback(std::function<void(const MemoryTrackerTester*)> fn)
		{
			func = std::move(fn);
		}

		std::function<void(const MemoryTrackerTester*)> func;
		const MemoryTrackerTester* tester;
	};

	void MemoryTrackerTester::Test()
	{
		constexpr auto npos = std::string::npos;
		constexpr auto testMemTypeSize = sizeof(TestMemType);
		
		MEM_INIT_LOGS("Test from MemoryTracker_Test");

		for (auto i = 0; i < 5; ++i)
			const auto* const lifeTime = new TestMemType(this);

		for (auto i = 0; i < 5; ++i)
			delete new TestMemType(this);
		
		const auto scope = Make_Solo<TestMemType>(this);


		const auto status = TestMemType::GetHeapStatus();
		const auto lines = klib::kString::Split(status, "\n");

		const auto name = lines[0];
		VERIFY(name.find(typeid(TestMemType).name()) != npos);

		const auto countStr = lines[1];
		VERIFY(countStr.find("6") != npos);

		const auto sizeOfTestManagerStr = lines[2];
		VERIFY(sizeOfTestManagerStr.find(std::to_string(testMemTypeSize)) != npos);

		const auto sizeOfBlockStr = lines[3];
		VERIFY(sizeOfBlockStr.find(std::to_string(testMemTypeSize + memory::ControlBlockSize)) != npos);

		const auto totalSizeOFObjectMemoryStr = lines[4];
		VERIFY(totalSizeOFObjectMemoryStr.find(std::to_string(testMemTypeSize * 6)) != npos);

		const auto totalSizeOFBlockMemoryStr = lines[5];
		VERIFY(totalSizeOFBlockMemoryStr.find(std::to_string((testMemTypeSize + memory::ControlBlockSize) * 6)) != npos);

		scope->SetDestructorCallback([&](const MemoryTrackerTester* ptr)
		{
			VERIFY(ptr == this);
		});
	}
}
#endif