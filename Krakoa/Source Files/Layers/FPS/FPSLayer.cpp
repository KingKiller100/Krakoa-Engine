#include "Precompile.hpp"
#include "FPSLayer.hpp"

#include "../../Instrumentor.hpp"

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

		ImGui::Begin("Frames Per Seconds (FPS)");

		const auto fps = CalculateAverageFPS();
		ImGui::Text("Average FPS: %.2f fps", fps);

		ImGui::End();
	}

	USE_RESULT float FPSLayer::CalculateAverageFPS() const noexcept
	{
		KRK_PROFILE_FUNCTION();

		float sum = 0;
		for (auto frameTime : sampleTimes)
			sum += frameTime;

		const auto count = sampleTimes.size();
		return sum / count;
	}

}
