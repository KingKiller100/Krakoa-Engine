#pragma once

#include "../../EventsSystem/Event.hpp"

#include "../LayerBase.hpp"

namespace krakoa
{
	class KRAKOA_API ImGuiLayer : public LayerBase
	{
	public:
		ImGuiLayer();

		~ImGuiLayer() override;

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnEvent(events::Event& e) override;

		void BeginDraw();
		void EndDraw();

		void ToggleVisibility() noexcept;

	private:
		bool isShowing;
	};
}
