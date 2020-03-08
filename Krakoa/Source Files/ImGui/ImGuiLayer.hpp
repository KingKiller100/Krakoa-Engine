#pragma once

#include <Rendering/LayerBase.hpp>


namespace krakoa
{
	
class ImGuiLayer : public LayerBase
{
public:
	ImGuiLayer();

	~ImGuiLayer() override;
	
	void OnAttach() override;
	void OnDetach() override;
	void OnUpdate() override;
	void OnEvent(events::Event& e) override;
};
}
