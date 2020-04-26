#include "pch.hpp"
#include "UTFConverter_Test.hpp"

#include "../../Utility/String/kUTFStringConverter.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	UTFConverterTester::UTFConverterTester()
		: Tester("UTF Converter Test")
	{}

	UTFConverterTester::~UTFConverterTester()
		= default;

	void UTFConverterTester::Test()
	{
		std::string_view test = "test";
		const auto value = klib::kString::UTF8ConvertToUTF16(test);
		VERIFY(value == u"test");
		auto buf32 = klib::kString::Convert<char32_t>(test);
		VERIFY(buf32 == U"test");
	}
}
#endif
