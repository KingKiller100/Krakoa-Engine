#include "Precompile.hpp"
#include "ProfilerStack.hpp"

namespace krakoa
{

	std::unique_ptr<std::stack<klib::kProfiler::ProfilerResult>>
		ProfilerStack::profilerResults =
		std::make_unique<std::stack<klib::kProfiler::ProfilerResult>>();

	std::stack<klib::kProfiler::ProfilerResult>& ProfilerStack::GetStack()
	{
		return *profilerResults;
	}
}
