#pragma once

#include "../HelperMacros.hpp"

#include "../Utility/Profiler/kProfiler.hpp"

#include <string>
#include <vector>

#ifdef TESTING_ENABLED
namespace kTest
{
	class SpeedTestBase
	{
	public:
		SpeedTestBase(const char* name) noexcept;
		~SpeedTestBase();

		void Run() noexcept;
		void SendResult(const std::string_view& result) noexcept;

		const std::string& GetName() const;

	private:
		virtual void Test() = 0;

	private:
		std::string name;
		std::vector<klib::kProfiler::ProfilerResult> profilerResults;
	};
}
#endif

#define PROFILE_SCOPE(profilee) auto profiler##__LINE__ = klib::kProfiler::Profiler(profilee, [&profilerResults](const klib::kProfiler::ProfilerResult& res)\
		{\
			profilerResults.push_back(res);\
		});\
