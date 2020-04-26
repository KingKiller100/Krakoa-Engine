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
		KRK_PROFILE_FUNCTION();
		for (auto layer : layerStack)
		{
			delete layer;
			layer = nullptr;
		}
	}

	void LayerStacker::PushLayer(LayerBase* layer)
	{
		KRK_PROFILE_FUNCTION();

		layerStack.emplace(layerStack.begin() + layerIterIndex, layer);
		layerIterIndex++;
		layer->OnAttach();
	}

	void LayerStacker::PushOverlay(LayerBase* overlay)
	{
		KRK_PROFILE_FUNCTION();

		layerStack.emplace_back(overlay);
		overlay->OnAttach();
	}

	void LayerStacker::PopLayer(LayerBase* layer)
	{
		KRK_PROFILE_FUNCTION();

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
		KRK_PROFILE_FUNCTION();

		const auto iter = std::find(begin(), end(), overlay);
		if (iter != end())
		{
			(*iter)->OnDetach();
			layerStack.erase(iter);
		}
	}

	std::vector<LayerBase*>::iterator LayerStacker::begin()
	{
		KRK_PROFILE_FUNCTION();

		return layerStack.begin();
	}

	const std::vector<LayerBase*>::const_iterator LayerStacker::begin() const
	{
		KRK_PROFILE_FUNCTION();

		return layerStack.cbegin();
	}

	std::vector<LayerBase*>::iterator LayerStacker::end()
	{
		KRK_PROFILE_FUNCTION();

		return layerStack.end();
	}

	const std::vector<LayerBase*>::const_iterator LayerStacker::end() const
	{
		KRK_PROFILE_FUNCTION();

		return layerStack.cend();
	}

	void LayerStacker::OnUpdate(const float deltaTime) const noexcept
	{
		KRK_PROFILE_FUNCTION();

		for (auto& layer : layerStack)
			(layer)->OnUpdate(deltaTime);
	}

	void LayerStacker::OnEvent(krakoa::events::Event & e) const noexcept
	{
		KRK_PROFILE_FUNCTION();

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
		KRK_PROFILE_FUNCTION();

		for (auto& layer : layerStack)
			(layer)->OnRender();
	}
}
