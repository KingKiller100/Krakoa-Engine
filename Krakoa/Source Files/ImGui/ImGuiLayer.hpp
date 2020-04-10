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
		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnEvent(events::Event& e) override;

		void BeginDraw();
		void EndDraw();

	private:
		float time;
	};
}
