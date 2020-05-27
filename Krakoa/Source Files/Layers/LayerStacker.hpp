#pragma once

#include "../Core/EngineConfig.hpp"

#include "../Events System/Event.hpp"

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

		std::vector<LayerBase*>::iterator begin();
		std::vector<LayerBase*>::iterator end();

		const std::vector<LayerBase*>::const_iterator begin() const ;
		const std::vector<LayerBase*>::const_iterator end() const;

		void OnUpdate(const float deltaTime) const noexcept;
		void OnEvent(krakoa::events::Event& e) const noexcept;
		void OnRender() const noexcept;

	private:
		std::vector<LayerBase*> layerStack;
		unsigned layerIterIndex;
	};
}
