#pragma once

#include <Core/EngineCore.hpp>
#include <Rendering/LayerBase.hpp>

#include <vector>
#include <memory>



class KRAKOA_API LayerStacker
{
public:
	LayerStacker();
	~LayerStacker();

	void PushLayer(LayerBase* layer);
	void PushOverlay(LayerBase* overlay);
	void PopLayer(LayerBase* layer);
	void PopOverlay(LayerBase* overlay);
	
	std::vector<std::shared_ptr<LayerBase>>::iterator Begin();
	std::vector<std::shared_ptr<LayerBase>>::iterator End();

private:
	std::vector<std::shared_ptr<LayerBase>> layerStack;
	std::vector<std::shared_ptr<LayerBase>>::iterator stackInserter;
};
