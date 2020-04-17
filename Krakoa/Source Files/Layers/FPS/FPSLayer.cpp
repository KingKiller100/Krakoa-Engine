#include "Precompile.hpp"
#include "FPSLayer.hpp"

#include <imgui.h>

#include <Maths/kAlgorithms.hpp>

namespace krakoa
{

	FPSLayer::FPSLayer() noexcept
		: LayerBase("FPS Layer")
	{}

	FPSLayer::~FPSLayer()
		= default;

	void FPSLayer::OnAttach()
	{}

	void FPSLayer::OnDetach()
	{}

	void FPSLayer::OnUpdate(float deltaTime)
	{
		currentIdx = kmaths::modulus(currentIdx, sampleTimes.size());
		sampleTimes[currentIdx++] = CAST(unsigned, (1.f / deltaTime));
	}

	void FPSLayer::OnRender()
	{
		ImGui::Begin("Frames Per Seconds (FPS)");

		const auto fps = CalculateAverageFPS();
		ImGui::Text("Average FPS: %.2f", fps);

		ImGui::End();
	}

	void FPSLayer::OnEvent(events::Event& e)
	{}

	float FPSLayer::CalculateAverageFPS() const noexcept
	{
		decltype(sampleTimes)::value_type sum = 0;

		for (auto frameTime : sampleTimes)
			sum += frameTime;

		const auto count = sampleTimes.size();
		return sum / count;
	}

}
