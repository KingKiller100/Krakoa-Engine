#include <Precompile.hpp>
#include <Rendering/LayerStacker.hpp>

#include <Rendering/LayerBase.hpp>

namespace krakoa
{
	LayerStacker::LayerStacker()
	{
		layerIter = begin();
	}

	LayerStacker::~LayerStacker()
	{
		for (auto layer : layerStack)
		{
			delete layer;
			layer = nullptr;
		}
	}

	void LayerStacker::PushLayer(LayerBase* layer)
	{
		layerIter = layerStack.emplace(layerIter, layer);
		layer->OnAttach();
	}

	void LayerStacker::PushOverlay(LayerBase* overlay)
	{
		layerStack.emplace_back(overlay);
		layerStack.back()->OnAttach();
	}

	void LayerStacker::PopLayer(LayerBase* layer)
	{
		const auto iter = std::find(begin(), end(), layer);
		if (iter != end())
		{
			(*iter)->OnDetach();
			layerStack.erase(iter);
			--layerIter;
		}
	}

	void LayerStacker::PopOverlay(LayerBase* overlay)
	{
		const auto iter = std::find(begin(), end(), overlay);
		if (iter != end())
		{
			(*iter)->OnDetach();
			layerStack.erase(iter);
		}
	}

	std::vector<LayerBase*>::iterator LayerStacker::begin()
	{
		return layerStack.begin();
	}

	std::vector<LayerBase*>::iterator LayerStacker::end()
	{
		return layerStack.end();
	}
}
