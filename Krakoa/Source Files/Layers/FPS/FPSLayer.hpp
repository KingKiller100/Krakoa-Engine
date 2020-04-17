#pragma once
#include "../LayerBase.hpp"

#include <array>

namespace krakoa
{
	class FPSLayer : public LayerBase
	{
	public:
		FPSLayer() noexcept;
		~FPSLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnEvent(events::Event& e) override;

	private:
		float CalculateAverageFPS() const noexcept;

	private:
		std::array<unsigned, 100> sampleTimes;
		size_t currentIdx;
	};
}