#pragma once

#include "../Core/EngineMacros.hpp"

#include "../EventsSystem/Event.hpp"

#include <vector>


namespace krakoa
{
	class KRAKOA_API LayerBase;
	class KRAKOA_API LayerStacker
	{
	public:
		LayerStacker();
		~LayerStacker();

		void PushLayer(LayerBase* layer);
		void PushOverlay(LayerBase* overlay);
		void PopLayer(LayerBase* layer);
		void PopOverlay(LayerBase* overlay);

		std::vector<LayerBase*>::reverse_iterator begin();
		std::vector<LayerBase*>::reverse_iterator end();

		const std::vector<LayerBase*>::const_reverse_iterator begin() const ;
		const std::vector<LayerBase*>::const_reverse_iterator end() const;

		void OnUpdate(const float deltaTime) const noexcept;
		void OnEvent(events::Event& e) const noexcept;
		void OnRender() const;

	private:
		std::vector<LayerBase*> layerStack;
		unsigned layerIterIndex;
	};
}
