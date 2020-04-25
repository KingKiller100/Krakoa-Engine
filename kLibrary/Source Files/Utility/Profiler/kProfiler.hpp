#pragma once

#include "../Timer/kTimer.hpp"

#include <stack>

namespace klib::kProfiler
{
	template<typename ProfilerFunc>
	class Profiler
	{
	private:
		using Func = ProfilerFunc;

	public:
		Profiler(const char* name, Func&& func)
			: name(name), timer("Profiler"), isRunning(true),
			timerFunc(std::forward<Func&&>(func))
		{}

		~Profiler()
		{
			if (isRunning)
				Stop();
		}

		const char* GetName() const
		{
			return name;
		}

	private:
		void Stop()
		{
			isRunning = false;
			const auto result = timer.GetLifeTime<kTime::Millis>();
			timerFunc({ name, result });
		}

	private:
		const char* name;
		kTime::HighAccuracyTimerf timer;
		bool isRunning;
		Func timerFunc;
	};

	struct ProfilerResult
	{
		const char * name;
		float result;
	};
}

#define PUSH_PROFILER(stack, name) klib::kProfiler::Profiler profiler##__LINE__(name, [&](const klib::kProfiler::ProfilerResult& profileResult) { stack.push(profileResult); })
#define EMPLACE_PROFILER(list, name) klib::kProfiler::Profiler profiler##__LINE__(name, [&](const klib::kProfiler::ProfilerResult& profileResult) { list.push_back(profileResult); })
