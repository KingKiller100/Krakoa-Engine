#include <pch.hpp>
#include <Testing/Utility Tests/FormatToString_Test.hpp>

#include <Maths/kAlgorithms.hpp>
#include <Utility/Format/kFormatToString.hpp>

namespace kTest::utility
{
	FormatToStringTester::FormatToStringTester()
		: Tester("Formatting To String Test")
	{	}

	FormatToStringTester::~FormatToStringTester()
		= default;

	void FormatToStringTester::Test()
	{
		FormatToStringTest();
		SingleObjectToStringTest();
	}

	void FormatToStringTester::FormatToStringTest()
	{
		auto tempIntPtr = new int(75);

		const auto testStr  = util::kFormat::FormatToString("This test %d ", 1U);
		const auto testStr2 = util::kFormat::FormatToString("will all %s printf function format specifiers like with string literals ", "work");
		const auto testStr3 = util::kFormat::FormatToString("and with different numerical types such as float %02.03f, ", float(kMaths::consts::TAU));
		const auto testStr4 = util::kFormat::FormatToString("doubles %02.07f, ", double(kMaths::consts::E));
		const auto testStr5 = util::kFormat::FormatToString("signed (%d) or unsigned integers (%u), ", -50, 200U);
		const auto testStr6 = util::kFormat::FormatToString("pointer addresses i.e. %p (random int ptr address)", tempIntPtr);

		VERIFY(testStr == "This test 1 ");
		VERIFY(testStr2 == "will all work printf function format specifiers like with string literals ");
		VERIFY(testStr3 == "and with different numerical types such as float 6.283, ");
		VERIFY(testStr4 == "doubles 2.7182818, ");
		VERIFY(testStr5 == "signed (-50) or unsigned integers (200), ");
		VERIFY(testStr6.find("pointer addresses ") != std::string::npos);
	}

	void FormatToStringTester::SingleObjectToStringTest()
	{
		const auto test = util::kFormat::SingleObjectToString<char, unsigned short>(980);

		VERIFY(test == "980");
	}
	
}