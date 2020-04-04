#include "pch.hpp"
#include "FormatToString_Test.hpp"

#include "../../Maths/Constants.hpp"
#include "../../Utility/Format/kFormatToString.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	FormatToStringTester::FormatToStringTester()
		: Tester("Formatting To String Test")
	{	}

	FormatToStringTester::~FormatToStringTester()
		= default;

	void FormatToStringTester::Test()
	{
		VERIFY(FormatToStringTest() == true);
		VERIFY(SingleObjectToStringTest() == true);
	}

	bool FormatToStringTester::FormatToStringTest()
	{
		using namespace klib::kFormat;

		auto tempIntPtr = std::make_unique<int>(76);

		const auto testStr  = ToString("This test %d ", 1U);
		const auto testStr2 = ToString("will all %s printf function format specifiers like with string literals ", "work");
		const auto testStr3 = ToString("and with different numerical types such as float %02.03f, ", float(kmaths::constants::TAU));
		const auto testStr4 = ToString("doubles %2.7f, ", double(kmaths::constants::E));
		const auto testStr5 = ToString("signed (%d) or unsigned integers (%u), ", -50, 200U);
		const auto testStr6 = ToString("pointer addresses i.e. %p (random int ptr address)", tempIntPtr.get());

		VERIFY(testStr == "This test 1 ");
		VERIFY(testStr2 == "will all work printf function format specifiers like with string literals ");
		VERIFY(testStr3 == "and with different numerical types such as float 6.283, ");
		VERIFY(testStr4 == "doubles 2.7182818, ");
		VERIFY(testStr5 == "signed (-50) or unsigned integers (200), ");
		VERIFY(testStr6.find("pointer addresses ") != std::string::npos);

		return success;
	}

	bool FormatToStringTester::SingleObjectToStringTest()
	{
		const auto test = klib::kFormat::ToString<char>(980u);
		VERIFY(test == "980");
		return success;
	}
}
#endif
