#pragma once

#include "../Core/EngineCore.hpp"

#include <vector>


namespace krakoa
{
#if defined (_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable:4251)

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

		void OnUpdate() const noexcept;
		void OnRender() const noexcept;

	private:
		std::vector<LayerBase*> layerStack;
		unsigned layerIterIndex;
	};
#	pragma warning(pop)
#endif
}
