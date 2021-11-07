#include "Precompile.hpp"
#include "LayerStacker.hpp"

#include "LayerBase.hpp"

#include "../Debug/Instrumentor.hpp"

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
		layerStack.clear();
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
		
		const auto iter = std::find(layerStack.begin(), layerStack.end(), layer);
		if (iter != layerStack.end())
		{
			(*iter)->OnDetach();
			layerStack.erase(iter);
			--layerIterIndex;
		}
	}

	void LayerStacker::PopOverlay(LayerBase* overlay)
	{
		KRK_PROFILE_FUNCTION();

		const auto iter = std::find(layerStack.begin(), layerStack.end(), overlay);
		if (iter != layerStack.end())
		{
			(*iter)->OnDetach();
			layerStack.erase(iter);
		}
	}

	std::vector<LayerBase*>::reverse_iterator LayerStacker::begin()
	{
		KRK_PROFILE_FUNCTION();

		return layerStack.rbegin();
	}

	const std::vector<LayerBase*>::const_reverse_iterator LayerStacker::begin() const
	{
		KRK_PROFILE_FUNCTION();

		return layerStack.crbegin();
	}

	std::vector<LayerBase*>::reverse_iterator LayerStacker::end()
	{
		KRK_PROFILE_FUNCTION();

		return layerStack.rend();
	}

	const std::vector<LayerBase*>::const_reverse_iterator LayerStacker::end() const
	{
		KRK_PROFILE_FUNCTION();

		return layerStack.crend();
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
		
		for (auto layer_iter = begin();
			layer_iter != end(); 
			++layer_iter)
		{
			(*layer_iter)->OnEvent(e);

			if (e.isHandled())
			{
				break;
			}
		}
	}

	void LayerStacker::OnRender() const
	{
		KRK_PROFILE_FUNCTION();

		for (auto& layer : layerStack)
			(layer)->OnRender();
	}
}
