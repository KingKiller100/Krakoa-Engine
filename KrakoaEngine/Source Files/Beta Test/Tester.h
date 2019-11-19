#pragma once

#include "../Core/EngineCore.h"

#include "../Utility/Format To String/kFormatToString.h"

namespace kTest
{
	class KRAKOA_API Tester
	{
	public:
		Tester(const char* name) noexcept;
		Tester(Tester&& other) noexcept;
		Tester& operator=(Tester&& other) noexcept;

		virtual ~Tester();

		constexpr const char* GetName() const;
		
		bool Run();

		//bool VerifyResults(const bool )

		// Deleted Funcs
		Tester(const Tester& other) = delete;
		Tester& operator=(const Tester& other) = delete;
		
	private:
		virtual bool Test() = 0;
		
	protected:
		const char* name;
	};
	
	 // If results are wrong, change name to failed test function signature and line, else continues to next line
#define VerifyResults(test)\
	if ((test) == false)\
	{\
		this->name = util::kFormatToString::FormatToString("%s Test:  Line %d     Function: %s", this->name, __LINE__, __FUNCSIG__).c_str();\
		return false; \
	}\

}
