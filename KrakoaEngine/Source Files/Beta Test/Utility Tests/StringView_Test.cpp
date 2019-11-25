#include "Precompile.h"
#include "StringView_Test.h"

#include "../../Utility/String View/StringView.h"


namespace kTest::utility
{
	using namespace util::kString_View;
	
	StringViewTester::StringViewTester()
		:Tester("StringView Test")
	{	}

	StringViewTester::~StringViewTester()
		= default;

	void StringViewTester::Test()
{
		const auto txt = "Char String";

		std::string_view test = txt; // When I need to test speed
		/*test.substr();*/
		StringView s = txt;
		VERIFY(s.Data() == "Char String" && s.Length() == 11)
		
		wStringView s2(L"Wide Char String");
		VERIFY(s2.Data() == L"Wide Char String" && s2.Length() == 16);
				
		u32StringView s3 = U"Unsigned 32-Bit String";
		VERIFY(s3.Data() == U"Unsigned 32-Bit String" && s3.Length() == 22)

		u16StringView s4 = u"Unsigned 16-Bit String";
		VERIFY(s4.Data() == u"Unsigned 16-Bit String" && s3.Length() == 22)
		
		//test.remove_prefix(3);
		//VERIFY(test.compare("r String") == 0);
		
		s.RemovePrefix(3);
		VERIFY(s.Compare("r String") == 0);

		s.RemoveSuffix(1);
		VERIFY(s.Length() == 7);

		const auto rPos = s.FirstInstanceOf('r');
		VERIFY(rPos != StringView::npos && rPos == 0);

		const auto ngPos = s.FirstInstanceOf("ng", 2);
		VERIFY(ngPos != StringView::npos && ngPos == 6);
		
		const auto badGPos = s.FirstInstanceOf('g', 2, 3);
		VERIFY(badGPos == StringView::npos);

		const auto Pos3 = s4.LastInstanceOf(L'6');
		VERIFY(Pos3 == 11);

		auto k = s2.Back();
		VERIFY(k == L'g');

		k = s2.Front();
		VERIFY(k == L'W');

		wStringView s5(L"Empty");
		s2.Swap(s5);
		VERIFY(s2 == L"Empty");
		VERIFY(s5 == L"Wide Char String");

		const auto ptr = s2.Data();
		VERIFY(ptr == L"Empty")

		s2.Clear();
		VERIFY(s2.Length() == 0 && s2.Data() == nullptr);
		VERIFY(s2.Empty() == true);
		
		auto v = s3.Substr(5);
		VERIFY(v.Compare(U"g Tester") && v);

		auto temp = s.Substr(s.Length());
		VERIFY(s.IsEqual(temp));
	}
}


