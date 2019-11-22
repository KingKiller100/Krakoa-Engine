#include "Precompile.h"
#include "FormatToString_Test.h"

#include "../../Utility/Format/kFormatToString.h"
#include "../../Utility/Maths/Constants.h"

namespace kTest::utility
{
	using namespace util;

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

		const auto testStr = kFormatToString::FormatToString("This test %d ", 1U);
		const auto testStr2 = kFormatToString::FormatToString("will all %s printf function format specifiers like with string literals ", "work");
		const auto testStr3 = kFormatToString::FormatToString("and with different numerical types such as float %02.03f, ", float(kMaths::TAU));
		const auto testStr4 = kFormatToString::FormatToString("doubles %02.07f, ", double(kMaths::E));
		const auto testStr5 = kFormatToString::FormatToString("signed or unsigned integers %d/%u, ", -50, 200U);
		const auto testStr6 = kFormatToString::FormatToString("pointer addresses %p ", tempIntPtr);
		const auto testStr7 = "etc..";

		VERIFY(testStr == "This test 1 ");
		VERIFY(testStr2 == "will work with string literals ");
		VERIFY(testStr3 == "with different types such as float 06.282, ");
		VERIFY(testStr4 == "doubles 2.7182818, ");
		VERIFY(testStr5 == "signed or unsigned ints 50, ");
		VERIFY(testStr6.find("pointer addresses ") != std::string::npos);
		VERIFY(testStr7 == "etc...")
	}

	void FormatToStringTester::SingleObjectToStringTest()
	{
		const auto test = kFormatToString::SingleObjectToString<char, unsigned short>(980);

		VERIFY(test == "980");
	}
	
}