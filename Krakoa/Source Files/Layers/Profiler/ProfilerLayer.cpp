#include "Precompile.hpp"
#include "ProfilerLayer.hpp"

#include <imgui.h>

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
		auto& profilerResults = ProfilerStack::GetStack();
		while (!profilerResults.empty())
		{
			auto& result = profilerResults.top();
			outputText.append(result.name);
			outputText.append(":   %.3fms");
			ImGui::Text(outputText.data(), result.result);
			profilerResults.pop();
			outputText.clear();
		} 

		ImGui::End();
	}

	void ProfilerLayer::OnEvent(events::Event& e)
	{}

}