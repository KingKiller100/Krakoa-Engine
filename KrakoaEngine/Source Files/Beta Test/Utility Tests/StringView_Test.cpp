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
		auto hooper2Text = "hooper2";
		StringView s = hooper2Text;
		wStringView s2(L"string literal");
		u32StringView s3 = U"String Tester";
		s.RemovePrefix(3);
		s.RemoveSuffix(1);
		auto pos = s.FirstInstanceOf('e');
		auto pos1 = s.FirstInstanceOf('g', 1, 2);
		auto pos2 = s.FirstInstanceOf("er");
		auto k = s2.Back();
		k = s2.Front();
		wStringView s4(L"Empty");
		s2.Swap(s4);
		auto ptr = s2.Data();
		s2.Clear();
		auto b = s2.Empty();
		auto l = s3.Length();
		auto v = s3.Substr(5);
		auto temp = s.Substr(s.Length());
		auto h = s.IsEqual(temp);
	}
}


