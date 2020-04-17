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
	{
		for (auto& frameTime : sampleTimes)
		{
			frameTime = 0;
		}
	}

	void FPSLayer::OnDetach()
	{}

	void FPSLayer::OnUpdate(float deltaTime)
	{
		currentIdx = kmaths::modulus(currentIdx, sampleTimes.size());
		sampleTimes[currentIdx++] = 1u / deltaTime;
	}

	void FPSLayer::OnRender()
	{
		ImGui::Begin("Frames Per Seconds (FPS)");

		const auto fps = CalculateAverageFPS();
		ImGui::Text("Average FPS: %.2f", fps + 1);

		ImGui::End();
	}

	void FPSLayer::OnEvent(events::Event& e)
	{}

	float FPSLayer::CalculateAverageFPS() const noexcept
	{
		float sum = 0;

		for (auto frameTime : sampleTimes)
			sum += frameTime;

		const auto count = sampleTimes.size();
		return sum / count;
	}

}
