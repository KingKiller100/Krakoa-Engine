#include "Precompile.hpp"
#include "FPSLayer.hpp"

#include "../../Debug/Instrumentor.hpp"
#include "../../UI/ImGui/UI.hpp"

#include <Maths/kAlgorithms.hpp>
#include <Utility/String/kToString.hpp>
#include <Template/kArthimeticOperators.hpp>

namespace krakoa
{
	FPSLayer::FPSLayer() noexcept
		: LayerBase("FPS Layer")
	{}

	FPSLayer::~FPSLayer()
		= default;

	void FPSLayer::OnAttach()
	{
		KRK_PROFILE_FUNCTION();

		for (auto& frameTime : sampleTimes)
		{
			frameTime = 0;
		}
	}

	void FPSLayer::OnDetach()
	{}

	void FPSLayer::OnUpdate(float deltaTime)
	{
		KRK_PROFILE_FUNCTION();

		currentIdx = kmaths::Modulus(currentIdx, sampleTimes.size());
		sampleTimes[currentIdx++] = CAST(unsigned, 1.f / deltaTime);
	}

	void FPSLayer::OnRender()
	{
		KRK_PROFILE_FUNCTION();

		ui::DrawPanel("Frames Per Seconds (FPS)",
			[&]()
			{
				const auto fps = CalculateAverageFPS();
				ui::DrawRawText(klib::ToString("Average FPS: {0:2} fps", fps));
			});
	}

	USE_RESULT float FPSLayer::CalculateAverageFPS() const noexcept
	{
		KRK_PROFILE_FUNCTION();

		const float sum = klib::kTemplate::ArithmeticOperators::Addition(sampleTimes, 0);
		const auto count = sampleTimes.size();
		const auto avg = sum / count;

		return avg;
	}

}
