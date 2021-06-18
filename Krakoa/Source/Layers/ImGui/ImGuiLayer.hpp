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
		void EndDraw() const;

		void ToggleVisibility() noexcept;

		bool GetBlockEventsStatus() const noexcept;
		void BlockEvents();
		void UnblockEvents();

	private:
		void SetDemoWindowStateFromConfig();
		
	private:
		bool demoWindowState;
		bool isShowing;
		bool isBlockingEvents;
	};
}
