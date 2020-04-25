#pragma once
#include "../LayerBase.hpp"

#include "ProfilerStack.hpp"

#include <stack>

namespace krakoa
{
	class ProfilerLayer : public LayerBase
	{
	public:
		ProfilerLayer() noexcept;
		~ProfilerLayer();

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnEvent(events::Event& e) override;
	};
}