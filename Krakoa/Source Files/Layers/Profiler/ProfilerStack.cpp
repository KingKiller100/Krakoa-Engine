#include "Precompile.hpp"
#include "ProfilerStack.hpp"

namespace krakoa
{

	std::unique_ptr<std::stack<klib::kProfiler::ProfilerResult>>
		ProfilerStack::profilerResults =
		std::make_unique<std::stack<klib::kProfiler::ProfilerResult>>();

	std::unique_ptr<std::vector<klib::kProfiler::ProfilerResult>>
		ProfilerList::profilerResults =
		std::make_unique<std::vector<klib::kProfiler::ProfilerResult>>();
}
