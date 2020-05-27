#include "pch.hpp"
#include "StringManipulation_Test.hpp"

#include "../../Source Files/Utility/String/kStringManipulation.hpp"

#ifdef TESTING_ENABLED
namespace kTest::utility
{
	StringManipulationTester::StringManipulationTester()
		: Tester("String Manipulator Test")
	{}

	StringManipulationTester::~StringManipulationTester()
		= default;

	void StringManipulationTester::Test()
	{
		VERIFY_MULTI_INIT();
		VERIFY_MULTI(ToWriterAndToReaderTest());
		VERIFY_MULTI(ReplaceTest());
		VERIFY_MULTI(SplitTest());
		VERIFY_MULTI_END();
	}

	bool StringManipulationTester::ReplaceTest()
	{
		const auto text = klib::kString::StringWriter<char>("My name is Rob");
		const auto res = klib::kString::Replace(text, 'R', 'B');
		VERIFY(res.compare("My name is Bob") == 0);

		return success;
	}

	bool StringManipulationTester::SplitTest()
	{
		const klib::kString::StringWriter<char32_t> poem = U"The wheels on the bus go round and round";

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

		return success;
	}

	bool StringManipulationTester::ToWriterAndToReaderTest()
	{
		{
			constexpr auto string = "String";

			const auto writer = klib::kString::ToWriter(string);
			const auto reader = klib::kString::ToReader(string);

			VERIFY(writer == string);
			VERIFY(reader == string);

			using CharType = ONLY_TYPE(decltype(string));
			using WriterType = ONLY_TYPE(decltype(writer));
			using ReaderType = ONLY_TYPE(decltype(reader));

			constexpr auto sameWriterType = std::is_same_v<WriterType, std::basic_string<CharType>>;
			constexpr auto sameReaderType = std::is_same_v<ReaderType, std::basic_string_view<CharType>>;
			constexpr auto not_match_W_R = !std::is_same_v<WriterType, ReaderType>;
			VERIFY_COMPILE_TIME(sameWriterType);
			VERIFY_COMPILE_TIME(sameReaderType);
			VERIFY_COMPILE_TIME(not_match_W_R);
		}

		{
			constexpr auto string = L"String";

			const auto writer = klib::kString::ToWriter(string);
			const auto reader = klib::kString::ToReader(string);

			VERIFY(writer == string);
			VERIFY(reader == string);

			using CharType = ONLY_TYPE(decltype(string));
			using WriterType = ONLY_TYPE(decltype(writer));
			using ReaderType = ONLY_TYPE(decltype(reader));

			constexpr auto sameWriterType = std::is_same_v<WriterType, std::basic_string<CharType>>;
			constexpr auto sameReaderType = std::is_same_v<ReaderType, std::basic_string_view<CharType>>;
			constexpr auto not_match_W_R = !std::is_same_v<WriterType, ReaderType>;
			VERIFY_COMPILE_TIME(sameWriterType);
			VERIFY_COMPILE_TIME(sameReaderType);
			VERIFY_COMPILE_TIME(not_match_W_R);
		}

		{
			constexpr auto string = u"String";

			const auto writer = klib::kString::ToWriter(string);
			const auto reader = klib::kString::ToReader(string);

			VERIFY(writer == string);
			VERIFY(reader == string);

			using CharType = ONLY_TYPE(decltype(string));
			using WriterType = ONLY_TYPE(decltype(writer));
			using ReaderType = ONLY_TYPE(decltype(reader));

			constexpr auto sameWriterType = std::is_same_v<WriterType, std::basic_string<CharType>>;
			constexpr auto sameReaderType = std::is_same_v<ReaderType, std::basic_string_view<CharType>>;
			constexpr auto not_match_W_R = !std::is_same_v<WriterType, ReaderType>;
			VERIFY_COMPILE_TIME(sameWriterType);
			VERIFY_COMPILE_TIME(sameReaderType);
			VERIFY_COMPILE_TIME(not_match_W_R);
		}

		{
			constexpr auto string = U"String";

			const auto writer = klib::kString::ToWriter(string);
			const auto reader = klib::kString::ToReader(string);

			VERIFY(writer == string);
			VERIFY(reader == string);

			using CharType = ONLY_TYPE(decltype(string));
			using WriterType = ONLY_TYPE(decltype(writer));
			using ReaderType = ONLY_TYPE(decltype(reader));

			constexpr auto sameWriterType = std::is_same_v<WriterType, std::basic_string<CharType>>;
			constexpr auto sameReaderType = std::is_same_v<ReaderType, std::basic_string_view<CharType>>;
			constexpr auto not_match_W_R = !std::is_same_v<WriterType, ReaderType>;
			VERIFY_COMPILE_TIME(sameWriterType);
			VERIFY_COMPILE_TIME(sameReaderType);
			VERIFY_COMPILE_TIME(not_match_W_R);
		}

#ifdef __cpp_char8_t
		{
			constexpr auto string = u8"String";

			const auto writer = klib::kString::ToWriter(string);
			const auto reader = klib::kString::ToReader(string);

			VERIFY(writer == string);
			VERIFY(reader == string);

			using CharType = ONLY_TYPE(decltype(string));
			using WriterType = ONLY_TYPE(decltype(writer));
			using ReaderType = ONLY_TYPE(decltype(reader));

			constexpr auto sameWriterType = std::is_same_v<WriterType, std::basic_string<CharType>>;
			constexpr auto sameReaderType = std::is_same_v<ReaderType, std::basic_string_view<CharType>>;
			constexpr auto not_match_W_R = !std::is_same_v<WriterType, ReaderType>;
			VERIFY_COMPILE_TIME(sameWriterType);
			VERIFY_COMPILE_TIME(sameReaderType);
			VERIFY_COMPILE_TIME(not_match_W_R);
		}
#endif 

		return success;

	}
}
#endif