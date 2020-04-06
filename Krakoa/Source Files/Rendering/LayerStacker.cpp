#include "Precompile.hpp"
#include "LayerStacker.hpp"

#include "LayerBase.hpp"

namespace krakoa
{
	LayerStacker::LayerStacker()
		: layerIterIndex(0)
	{}

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
		layerStack.emplace(layerStack.begin() + layerIterIndex, layer);
		layerIterIndex++;
		layer->OnAttach();
	}

	void LayerStacker::PushOverlay(LayerBase* overlay)
	{
		layerStack.emplace_back(overlay);
		overlay->OnAttach();
	}

	void LayerStacker::PopLayer(LayerBase* layer)
	{
		const auto iter = std::find(begin(), end(), layer);
		if (iter != end())
		{
			(*iter)->OnDetach();
			layerStack.erase(iter);
			--layerIterIndex;
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

	void LayerStacker::OnUpdate() const noexcept
	{
		for (auto& layer : layerStack)
			(layer)->OnUpdate();
	}

	void LayerStacker::OnRender() const noexcept
	{
		for (auto& layer : layerStack)
			(layer)->OnRender();
	}
}
