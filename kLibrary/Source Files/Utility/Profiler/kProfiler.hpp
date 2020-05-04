#pragma once

#include "../Timer/kTimer.hpp"
#include <string>
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
		Profiler(const std::string_view& name, Func&& cb)
			: result({ name.data(), 0, 0, 0 }), isRunning(true),
			callback(std::forward<Func&&>(cb)), 
			timer("Profiler")
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

			callback(result);

			isRunning = false;
		}

	private:
		ProfilerResult result;
		bool isRunning;
		Func callback;

		kTime::Timer<int64_t> timer;
	};
}
