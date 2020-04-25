#pragma once

#include <Utility/Profiler/kProfiler.hpp>
#include <stack>
#include <vector>
#include <memory>

namespace krakoa
{
	class ProfilerStack
	{
	public:
		static inline std::stack<klib::kProfiler::ProfilerResult>& GetStack() { return *profilerResults; }

	private:
		static std::unique_ptr<std::stack<klib::kProfiler::ProfilerResult>> profilerResults;
	};

	class ProfilerList
	{
	public:
		static inline std::vector<klib::kProfiler::ProfilerResult>& GetList() { return *profilerResults; }

	private:
		static std::unique_ptr<std::vector<klib::kProfiler::ProfilerResult>> profilerResults;
	};
}

#define PROFILE_SCOPE(name) EMPLACE_PROFILER(krakoa::ProfilerList::GetList(), name)
#define PROFILE_SCOPE_STACK(name) PUSH_PROFILER(krakoa::ProfilerStack::GetStack(), name)
