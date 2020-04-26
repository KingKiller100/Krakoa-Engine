#pragma once

#include "../Timer/kTimer.hpp"
#include <thread>

namespace klib::kProfiler
{
	struct ProfilerResult
	{
		const std::string name;
		int64_t start, end;
		uint32_t threadID;
	};

	template<typename ProfilerFunc>
	class Profiler
	{
	private:
		using Func = ProfilerFunc;

	public:
		Profiler(const char* name, Func&& func)
			: result({ name, 0, 0, 0 }), timer("Profiler"), isRunning(true),
			timerFunc(std::forward<Func&&>(func))
		{}

		~Profiler()
		{
			if (isRunning)
				Stop();
		}

	private:
		void Stop()
		{
			const auto endTimepoint = timer.Now<kTime::kUnits::Millis>();
			result.end = endTimepoint;
			result.start = timer.GetStartTime<kTime::kUnits::Millis>();
			result.threadID = (uint32_t)std::hash<std::thread::id>{}(std::this_thread::get_id());
			isRunning = false;

			timerFunc(result);
		}

	private:
		ProfilerResult result;
		kTime::Timer<int64_t> timer;
		bool isRunning;
		Func timerFunc;
	};
}

#define PUSH_PROFILER(stack, name) klib::kProfiler::Profiler profiler##__LINE__(name, [&](const klib::kProfiler::ProfilerResult& profileResult) { stack.push(profileResult); })
#define EMPLACE_PROFILER(list, name) klib::kProfiler::Profiler profiler##__LINE__(name, [&](const klib::kProfiler::ProfilerResult& profileResult) { list.push_back(profileResult); })
