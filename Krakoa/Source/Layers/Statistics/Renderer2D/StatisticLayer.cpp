#include "Precompile.hpp"
#include "StatisticLayer.hpp"

#include "../../../UI/ImGui/UI.hpp"
#include "../../../Debug/Instrumentor.hpp"
#include "../../../Graphics/2D/Renderer2D.hpp"

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

#if KRK_ENABLE_STATISTICS

		using namespace ui;

		DrawPanel("Renderer2D Statistics", []()
			{
				const auto& stats = graphics::Renderer2D::GetStats();
				DrawRawText("Total Draw Calls: {0}", stats.TotalDrawCalls());
				DrawRawText("Total Geometry: {0}", stats.TotalGeometryCount());

				DrawNewLine();
				DrawRawText("Quad Draw Calls: {0}", stats.quadDrawCallsCount);
				DrawRawText("Triangle Draw Calls: {0}", stats.triangleDrawCallsCount);

				DrawNewLine();
				DrawRawText("Quad Max Batch: {0}", graphics::batch::limits::quad::max);
				DrawRawText("Quad Count: {0}", stats.quadCount);
				DrawRawText("Quad Vertices Count: {0}", stats.TotalQuadVertexCount());
				DrawRawText("Quad Indices Count: {0}", stats.TotalQuadIndexCount());

				DrawNewLine();
				DrawRawText("Triangle Max Batch: {0}", graphics::batch::limits::triangle::max);
				DrawRawText("Triangle Count: {0}", stats.triangleCount);
				DrawRawText("Triangle Vertices Count: {0}", stats.TotalTriangleVertexCount());
				DrawRawText("Triangle Indices Count: {0}", stats.TotalTriangleIndexCount());
			});
#endif //KRK_ENABLE_STATISTICS
	}
}

