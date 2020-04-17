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

	const std::vector<LayerBase*>::const_iterator LayerStacker::begin() const
	{
		return layerStack.cbegin();
	}

	std::vector<LayerBase*>::iterator LayerStacker::end()
	{
		return layerStack.end();
	}

	const std::vector<LayerBase*>::const_iterator LayerStacker::end() const
	{
		return layerStack.cend();
	}

	void LayerStacker::OnUpdate(const float deltaTime) const noexcept
	{
		for (auto& layer : layerStack)
			(layer)->OnUpdate(deltaTime);
	}

	void LayerStacker::OnEvent(krakoa::events::Event & e) const noexcept
	{
		for (auto layer_iter = end(); layer_iter != begin();)
		{
			--layer_iter;
			(*layer_iter)->OnEvent(e);
			if (e.isHandled())
				break;
		}
	}

	void LayerStacker::OnRender() const noexcept
	{
		for (auto& layer : layerStack)
			(layer)->OnRender();
	}
}
