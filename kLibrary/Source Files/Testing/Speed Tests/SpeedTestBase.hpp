#pragma once

#include "../../HelperMacros.hpp"

#include "../../Utility/Profiler/kProfiler.hpp"

#include <string>
#include <vector>

#ifdef TESTING_ENABLED
namespace kTest::speed
{
	class SpeedTestBase
	{
	public:
		SpeedTestBase(const char* name) noexcept;
		~SpeedTestBase();

		void Run() noexcept;
		void SendResult(const std::string_view& result) noexcept;

		const std::string& GetName() const;

	protected:
		virtual void Test() = 0;

	protected:
		std::string name;
		std::vector<klib::kProfiler::ProfilerResult> profilerResults;
	};
}

#define PROFILE_SCOPE(profilee) auto profiler##__LINE__ = klib::kProfiler::Profiler(profilee, [&](const klib::kProfiler::ProfilerResult& res)\
		{\
			profilerResults.push_back(res);\
		});\

#endif
