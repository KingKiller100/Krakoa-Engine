#pragma once

#include <HelperMacros.hpp>

#include <Utility/Format/kFormatToString.hpp>
#include <Utility/File System/kFileSystem.hpp>

#include <string>
#include <sstream>

namespace kTest
{

//#pragma warning(push)
//#pragma warning(disable:4251)
//	PORT_STL template class KLIB_API std::basic_string<char>;
//	PORT_STL template class KLIB_API std::basic_string_view<char>;
//	PORT_STL template class KLIB_API std::basic_stringstream<char>;
//#pragma warning(pop)
	
	class Tester
	{
	public:
		constexpr Tester(const char* name) noexcept;
		Tester(Tester&& other) noexcept;
		Tester& operator=(Tester&& other) noexcept;

		virtual ~Tester();

		const char* GetName() const noexcept;
		std::string GetResult() const noexcept;
		
		bool Run() noexcept;

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
		this->result << util::kFormat::FormatToString("\tCondition: %s\n\tFile: %s\n\tFunction: %s\n\tLine: %d\n\n", #test, __FILE__, __FUNCSIG__, __LINE__);\
		this->success = false; \
	}\

}
