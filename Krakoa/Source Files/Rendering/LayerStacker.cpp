#include <Precompile.hpp>
#include <Rendering/LayerStacker.hpp>


namespace krakoa
{
	LayerStacker::LayerStacker()
		: layerIter(Begin())
	{
	}

	LayerStacker::~LayerStacker()
		= default;

	void LayerStacker::PushLayer(LayerBase* layer)
	{
		layerIter = layerStack.emplace(layerIter, layer);
	}

	void LayerStacker::PushOverlay(LayerBase* overlay)
	{
		layerStack.emplace_back(overlay);
	}

	void LayerStacker::PopLayer(LayerBase* layer)
	{
		auto iter = std::find(Begin(), End(), layer);
		if (iter != End())
		{
			layerStack.erase(iter);
			layerIter--;
		}
	}

	void LayerStacker::PopOverlay(LayerBase* overlay)
	{
		auto iter = std::find(Begin(), End(), overlay);
		if (iter != End())
		{
			layerStack.erase(iter);
		}
	}

	std::vector<LayerBase*>::iterator LayerStacker::Begin()
	{
		return layerStack.begin();
	}

	std::vector<LayerBase*>::iterator LayerStacker::End()
	{
		return layerStack.end();
	}
}
