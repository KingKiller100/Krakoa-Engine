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
		auto buf32 = new char32_t[test.size() + 1]{ U'\0' };
		klib::kString::Convert(test.data(), buf32);\
		std::u32string_view str32 = buf32;
		VERIFY(str32 == U"test");
		delete[] buf32;
	}
}
#endif
