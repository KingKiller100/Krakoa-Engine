#include "pch.hpp"
#include "StringManipulation_Test.hpp"

#include "../../Utility/String/kStringManipulation.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	StringManipulationTester::StringManipulationTester()
		: Tester("String Manipulator Test")
	{

	}

	StringManipulationTester::~StringManipulationTester()
		= default;


	void StringManipulationTester::Test()
	{
		auto text = klib::kString::StringWriter<char>("My name is Rob");
		const auto res = klib::kString::Replace(text, 'R', 'B');
		VERIFY(res.compare("My name is Bob") == 0);

		klib::kString::StringWriter<char32_t> poem = U"The wheels on the bus go round and round";
		const auto splits = klib::kString::Split<char32_t>(poem, U" ");
		VERIFY(splits[0].compare(U"The") == 0);
		VERIFY(splits[1].compare(U"wheels") == 0);
		VERIFY(splits[2].compare(U"on") == 0);
		VERIFY(splits[3].compare(U"the") == 0);
		VERIFY(splits[4].compare(U"bus") == 0);
		VERIFY(splits[5].compare(U"go") == 0);
		VERIFY(splits[6].compare(U"round") == 0);
		VERIFY(splits[7].compare(U"and") == 0);
		VERIFY(splits[8].compare(U"round") == 0);

		const auto splitsWithSpace = klib::kString::Split<char32_t>(poem, U" ", true);
		VERIFY(splitsWithSpace[0].compare(U"The ") == 0);
		VERIFY(splitsWithSpace[1].compare(U"wheels ") == 0);
		VERIFY(splitsWithSpace[2].compare(U"on ") == 0);
		VERIFY(splitsWithSpace[3].compare(U"the ") == 0);
		VERIFY(splitsWithSpace[4].compare(U"bus ") == 0);
		VERIFY(splitsWithSpace[5].compare(U"go ") == 0);
		VERIFY(splitsWithSpace[6].compare(U"round ") == 0);
		VERIFY(splitsWithSpace[7].compare(U"and ") == 0);
		VERIFY(splitsWithSpace[8].compare(U"round") == 0);
	}

}
#endif