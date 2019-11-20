#pragma once

#include "../Core/EngineCore.h"

#include "../Utility/Format To String/kFormatToString.h"
#include "../Utility/File System/kFileSystem.h"

#include <string>

namespace kTest
{

#if defined (_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
	EXPIMP_TEMPLATE template class KRAKOA_API std::basic_string<char>;
	EXPIMP_TEMPLATE template class KRAKOA_API std::basic_string_view<char>;
#pragma warning(pop)
#endif
	
	class KRAKOA_API Tester
	{
	public:
		Tester(const char* name) noexcept;
		Tester(Tester&& other) noexcept;
		Tester& operator=(Tester&& other) noexcept;

		virtual ~Tester();

		const char* GetName() const;
		
		bool Run();

		// Deleted Funcs
		Tester(const Tester& other) = delete;
		Tester& operator=(const Tester& other) = delete;
		
	private:
		virtual bool Test() = 0;
		
	protected:
		std::string name;
	};
	
	 // If results are wrong, change name to failed test function signature and line, else continues to next line
#define VERIFY(test)\
	if ((test) == false)\
	{\
		this->name = util::kFormatToString::FormatToString("Test Name: %s\n\t\t\tLine: %d\n\t\t\tFunction: %s\n\t\t\tCondition: %s", this->name.c_str(), __LINE__, FUNC_SIG, #test);\
		return false; \
	}\

}
