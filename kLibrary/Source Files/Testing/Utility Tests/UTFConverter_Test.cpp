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
		constexpr auto str16 = u"I'm a char32 ptr";
		const auto str32 = klib::kString::Convert<char32_t>(str16);
		const std::string_view test = "test";
		const auto value = klib::kString::Convert<char16_t>(test);
		VERIFY(value == u"test");
		auto buf32 = klib::kString::Convert<char32_t>(test);
		VERIFY(buf32 == U"test");
		const auto str = std::string(test);
		auto buf16str = klib::kString::Convert<char16_t>(str);
		VERIFY(buf16str == u"test");
	}
}
#endif
