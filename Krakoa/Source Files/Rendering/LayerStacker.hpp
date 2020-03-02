#pragma once

#include <Core/EngineCore.hpp>

#include <Rendering/LayerBase.hpp>

#include <vector>
#include <memory>


namespace krakoa
{
#if defined (_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4251)
	//EXPIMP_TEMPLATE template class KRAKOA_API std::vector<LayerBase*>;
	//EXPIMP_TEMPLATE template class KRAKOA_API std::_Vector_iterator<std::vector<LayerBase*>::_Scary_val>;

	
	class KRAKOA_API LayerStacker
	{
	public:
		LayerStacker();
		~LayerStacker();

		void PushLayer(LayerBase* layer);
		void PushOverlay(LayerBase* overlay);
		void PopLayer(LayerBase* layer);
		void PopOverlay(LayerBase* overlay);

		std::vector<LayerBase*>::iterator Begin();
		std::vector<LayerBase*>::iterator End();

	private:
		std::vector<LayerBase*> layerStack;
		std::vector<LayerBase*>::iterator layerIter;
	};
# pragma warning(pop)
#endif
}
