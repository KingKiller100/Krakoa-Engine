#pragma once

#include "../Core/EngineCore.h"

#include "../Utility/Format/kFormatToString.h"
#include "../Utility/File System/kFileSystem.h"

#include <string>
#include <sstream>

namespace kTest
{

#pragma warning(push)
#pragma warning(disable:4251)
	EXPIMP_TEMPLATE template class KRAKOA_API std::basic_string<char>;
	EXPIMP_TEMPLATE template class KRAKOA_API std::basic_string_view<char>;
	EXPIMP_TEMPLATE template class KRAKOA_API std::basic_stringstream<char>;
#pragma warning(pop)
	
	class KRAKOA_API Tester
	{
	public:
		Tester(const char* name) noexcept;
		Tester(Tester&& other) noexcept;
		Tester& operator=(Tester&& other) noexcept;

		virtual ~Tester();

		const char* GetName() const;
		std::string GetResult() const;
		
		bool Run();

		// Deleted Funcs
		Tester(const Tester& other) = delete;
		Tester& operator=(const Tester& other) = delete;
		
	private:
		virtual void Test() = 0;
		
	protected:
		std::string name;
		std::stringstream result;
		bool success;
	};
	
	 // If results are wrong, change name to failed test function signature and line, else continues to next line
#define VERIFY(test)\
	if ((test) == false)\
	{\
		this->result << util::kFormatToString::FormatToString("Failure: Test Name: %s\n\tCondition: %s\n\tFunction: %s\n\tCode Line: %d\n", this->name.c_str(), #test, FUNC_SIG, __LINE__);\
		this->success = false; \
	}\

}
