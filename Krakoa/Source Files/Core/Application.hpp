#pragma once

#include <iWindow.hpp>

#include <Core/EngineCore.hpp>
#include <Core/FPSCounter.hpp>
#include <Events System/Event.hpp>
#include <Events System/ApplicationEvent.hpp>
#include <Rendering/LayerStacker.hpp>

#include <memory>

namespace krakoa
{
#if defined (_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable:4251)
		EXPIMP_TEMPLATE template class KRAKOA_API std::unique_ptr<iWindow, std::default_delete<iWindow>>;

	class KRAKOA_API Application
	{
	public:
		Application();
		virtual ~Application();

		void Initialize();
		void Run();
		USE_RESULT constexpr bool IsRunning() const;
		virtual void Shutdown() {}

		iWindow& GetWindow() const;
		
		static Application& Get();

	protected:
		void PushLayer(LayerBase* layer);
		void PushOverlay(LayerBase* overlay);

	private:
		void OnEvent(events::Event& e);
		bool OnWindowClosed(events::WindowClosedEvent& e);

	protected:
		bool isRunning;
		std::unique_ptr<iWindow> window;
		FPSCounter fpsCounter;
		LayerStacker layerStack;

		static Application* instance;
	};

	Application* CreateApplication();
#	pragma warning(pop)
#endif
}