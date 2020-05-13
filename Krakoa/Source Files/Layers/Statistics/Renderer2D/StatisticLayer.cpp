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

		//constexpr auto size = sizeof(stats);
		
		ImGui::Begin("Renderer2D Statistics");
		
		ImGui::Text("Draw Calls: %zu", stats.drawCallsCount);

		ImGui::NewLine();
		ImGui::Text("Quad Count: %zu", stats.quadCount);
		ImGui::Text("Quad Vertices Count: %zu", stats.TotalQuadVertexCount());
		ImGui::Text("Quad Indices Count: %zu", stats.TotalQuadIndexCount());

		ImGui::NewLine();
		ImGui::Text("Triangle Count: %zu", stats.triangleCount);
		ImGui::Text("Triangle Vertices Count: %zu", stats.TotalTriangleVertexCount());
		ImGui::Text("Triangle Indices Count: %zu", stats.TotalTriangleIndexCount());
		
		ImGui::End();
#endif //ENABLE_STATISTICS
	}
}

