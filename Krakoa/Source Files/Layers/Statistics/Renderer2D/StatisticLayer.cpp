#include "Precompile.hpp"
#include "StatisticLayer.hpp"

#include "../../../Instrumentor.hpp"
#include "../../../Rendering/Renderer2D.hpp"

#include <imgui.h>

namespace krakoa
{
	Renderer2DStatistics::Renderer2DStatistics()
		: LayerBase("Renderer2D Statistics Layer")
	{}

	Renderer2DStatistics::~Renderer2DStatistics()
		= default;

	void Renderer2DStatistics::OnRender()
	{
		KRK_PROFILE_FUNCTION();
		
#if ENABLE_STATISTICS
		const auto& stats = krakoa::graphics::Renderer2D::GetStats();
		constexpr auto size = sizeof stats;
		ImGui::Begin("Renderer2D Statistics");
		ImGui::Text("Draw Calls: %zu", stats.drawCallsCount);
		ImGui::Text("Quad Count: %zu", stats.quadCount);
		ImGui::Text("Vertices Count: %zu", stats.TotalQuadVertexCount());
		ImGui::Text("Indices Count: %zu", stats.TotalQuadIndexCount());
		ImGui::End();
#endif //ENABLE_STATISTICS
	}
}

