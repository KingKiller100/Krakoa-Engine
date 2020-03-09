#pragma once

#include <Rendering/LayerBase.hpp>

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
		float time;
	};
}
