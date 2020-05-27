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

	private:
		USE_RESULT float CalculateAverageFPS() const noexcept;

	private:
		std::array<unsigned short, 100> sampleTimes;
		size_t currentIdx;
	};
}