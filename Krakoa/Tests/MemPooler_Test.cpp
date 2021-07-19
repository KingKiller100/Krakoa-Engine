#include "Precompile.hpp"
#include "MemPooler_Test.hpp"

#include "../Source/Patterns/MemPooler.hpp"

#include <array>

#ifdef KRAKOA_TEST
namespace krakoa::tests
{
	MemPoolerTester::MemPoolerTester()
		: TesterBase("MemPooler Test")
	{}

	MemPoolerTester::~MemPoolerTester()
		= default;

	class Testclass : public patterns::MemPooler<Testclass, 10>
	{
	public:
		Testclass(const size_t id = 0)
			: id(id)
		{}

		~Testclass() override = default;

	private:
		size_t id;
	};

	void MemPoolerTester::Prepare() noexcept
	{
		ADD_TEST(Test());
	}

	void MemPoolerTester::Test()
	{
		std::array<Testclass*, Testclass::MemoryPoolSize> list{};

		for (auto i = 0; i < list.size(); ++i)
			list[i] = new Testclass(i);

		auto status = Testclass::GetStatus();

		delete list.back();

		status = Testclass::GetStatus();
	}
}
#endif