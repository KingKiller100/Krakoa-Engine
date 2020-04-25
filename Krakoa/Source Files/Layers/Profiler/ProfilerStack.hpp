#pragma once

#include <Utility/Profiler/kProfiler.hpp>
#include <stack>
#include <memory>

namespace krakoa
{
	class ProfilerStack
	{
	public:
		static std::stack<klib::kProfiler::ProfilerResult>& GetStack();

	private:
		static std::unique_ptr<std::stack<klib::kProfiler::ProfilerResult>> profilerResults;
	};
}

#define PROFILE_SCOPE(name) PUSH_PROFILER(krakoa::ProfilerStack::GetStack(), name)
