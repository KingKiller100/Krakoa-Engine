#pragma once

#include "../Events System/KeyEvent.hpp"
#include "../Events System/MouseEvent.hpp"
#include "../Events System/ApplicationEvent.hpp"

#include "../Rendering/LayerBase.hpp"

namespace krakoa
{
	class KRAKOA_API ImGuiLayer : public LayerBase
	{
	public:
		ImGuiLayer();

		~ImGuiLayer() override;

		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void OnEvent(events::Event& e) override;

	private:
		bool OnMousePressedEvent(events::MouseButtonPressedEvent& e);
		bool OnMouseReleasedEvent(events::MouseButtonReleasedEvent& e);
		bool OnMouseMovedEvent(events::MouseMovedEvent& e);
		bool OnMouseScrolledEvent(events::MouseScrolledEvent& e);
		
		bool OnKeyPressedEvent(events::KeyPressedEvent& e);
		bool OnKeyReleasedEvent(events::KeyReleasedEvent& e);
		bool OnKeyTypedEvent(events::KeyTypedEvent& e);
		
		bool OnWindowResizedEvent(events::WindowResizeEvent& e);


	private:
		float time;
	};
}
