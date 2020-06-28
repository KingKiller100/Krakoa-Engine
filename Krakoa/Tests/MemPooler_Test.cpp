#include "Precompile.hpp"
#include "MemPooler_Test.hpp"

#include "../Source Files/Patterns/MemPooler.hpp"

#include <array>

#ifdef KRAKOA_TEST
namespace krakoa::tests
{
	MemPoolerTester::MemPoolerTester()
		: Tester("MemPooler Test")
	{}

	MemPoolerTester::~MemPoolerTester()
		= default;

	class Testclass : public patterns::MemPooler<Testclass, 10>
	{
	public:
		Testclass()
			= default;

		Testclass(const size_t id)
			: id(id)
		{}

		~Testclass()
			= default;

	private:
		size_t id;
	};

	void MemPoolerTester::Test()
	{
		std::array<Testclass*, Testclass::MemoryPoolSize> list{};

		for (auto i = 0; i < list.size(); ++i)
			list[i] = new Testclass(i);

		auto status = Testclass::GetStatus();

		delete list[list.size() - 1];

		status = Testclass::GetStatus();

	}
}
#endif