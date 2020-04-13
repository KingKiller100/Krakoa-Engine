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
	{

	}


	void StringManipulationTester::Test()
	{
		auto text = klib::kString::StringWriter<char>("My name is Rob");
		text = klib::kString::Replace(text, 'R', 'B');
		VERIFY(text.compare("My name is Bob") == 0);

		klib::kString::StringWriter<char> poem = "The wheels on the bus go round and round";
		const auto splits = klib::kString::Split<char>(poem, " ");
	}

}
#endif