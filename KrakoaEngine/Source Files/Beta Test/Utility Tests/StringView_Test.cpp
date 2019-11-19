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

	bool StringViewTester::Test()
	{
		auto hooper2Text = "hooper2";
		StringView s = hooper2Text;
		wStringView s2(L"string literal");
		u32StringView s3 = U"String Tester";
		
		s.RemovePrefix(3);
		VerifyResults(s.Data() == "per2");

		s.RemoveSuffix(1);
		VerifyResults(s.Length() == 3);

		auto ePos = s.FirstInstanceOf('e');
		VerifyResults(ePos != StringView::npos);

		auto gPos = s.FirstInstanceOf('g', 1, 2);
		VerifyResults(gPos == StringView::npos);
		
		auto erPos = s.FirstInstanceOf("er");
		VerifyResults(erPos != StringView::npos);

		auto k = s2.Back();
		VerifyResults(k == L'l');

		k = s2.Front();
		VerifyResults(k == L's');

		wStringView s4(L"Empty");
		s2.Swap(s4);
		VerifyResults(s2 == L"Empty");
		VerifyResults(s4 == L"string literal");

		auto ptr = s2.Data();
		VerifyResults(ptr == L"string literal")

		s2.Clear();
		VerifyResults(s2.Length() == 0 && s2.Data() == nullptr);
		VerifyResults(s2.Empty() == true);
		
		auto v = s3.Substr(5);
		VerifyResults(v == U"g Tester" && v);

		auto temp = s.Substr(s.Length());
		auto h = s.IsEqual(temp);

		return true;
	}
}


