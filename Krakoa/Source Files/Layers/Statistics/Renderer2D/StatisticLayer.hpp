#pragma once

#include "../../LayerBase.hpp"

namespace krakoa
{
	class Renderer2DStatistics : public LayerBase
	{
	public:
		Renderer2DStatistics();
		~Renderer2DStatistics() override;

		void OnRender() override;
	};
}
