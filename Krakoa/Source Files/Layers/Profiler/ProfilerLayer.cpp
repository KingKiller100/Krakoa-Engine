#include "Precompile.hpp"
#include "ProfilerLayer.hpp"

#include <imgui.h>

#include <algorithm>

namespace krakoa
{

	ProfilerLayer::ProfilerLayer() noexcept
		: LayerBase("Profiler Layer")
	{}

	ProfilerLayer::~ProfilerLayer()
		= default;

	void ProfilerLayer::OnAttach()
	{
	}

	void ProfilerLayer::OnDetach()
	{

	}

	void ProfilerLayer::OnUpdate(float deltaTime)
	{

	}

	void ProfilerLayer::OnRender()
	{
		std::string outputText;
		ImGui::Begin("Profiler");
		auto& profilerResults = ProfilerList::GetList();

		std::sort(profilerResults.begin(), profilerResults.end(), 
			[](klib::kProfiler::ProfilerResult& r1, klib::kProfiler::ProfilerResult& r2) {return r1.result > r2.result; });

		for (auto& result : profilerResults)
		{
			outputText.append(result.name);
			outputText.append(":   %.3fms");
			ImGui::Text(outputText.data(), result.result);
			outputText.clear();
		} 
		profilerResults.clear();

		ImGui::End();
	}

	void ProfilerLayer::OnEvent(events::Event& e)
	{}

}