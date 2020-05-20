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
		
		ImGui::Text("Total Draw Calls: %zu", stats.TotalDrawCalls());
		ImGui::Text("Total Geometry: %zu", stats.TotalGeometryCount());

		ImGui::NewLine();
		ImGui::Text("Quad Draw Calls: %zu", stats.quadDrawCallsCount);
		ImGui::Text("Triangle Draw Calls: %zu", stats.triangleDrawCallsCount);

		ImGui::NewLine();
		ImGui::Text("Quad Max Batch: %zu", graphics::batch::limits::quad::max);
		ImGui::Text("Quad Count: %zu", stats.quadCount);
		ImGui::Text("Quad Vertices Count: %zu", stats.TotalQuadVertexCount());
		ImGui::Text("Quad Indices Count: %zu", stats.TotalQuadIndexCount());

		ImGui::NewLine();
		ImGui::Text("Triangle Max Batch: %zu", graphics::batch::limits::triangle::max);
		ImGui::Text("Triangle Count: %zu", stats.triangleCount);
		ImGui::Text("Triangle Vertices Count: %zu", stats.TotalTriangleVertexCount());
		ImGui::Text("Triangle Indices Count: %zu", stats.TotalTriangleIndexCount());
		
		ImGui::End();
#endif //ENABLE_STATISTICS
	}
}

