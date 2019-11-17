#pragma once

#include "../Core/EngineCore.h"

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


		// Deleted Funcs
		Tester(const Tester& other) = delete;
		Tester& operator=(const Tester& other) = delete;
		
	private:
		virtual void Test() = 0;
		
	protected:
		const char* name;
	};

	constexpr const char* Tester::GetName() const
	{
		return name;
	}
}
