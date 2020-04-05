#pragma once

#include "EngineCore.hpp"
#include "FPSCounter.hpp"

// Window
#include "../iWindow.hpp"

// Pattern
#include "../Patterns/SimpleSingleton.hpp"

// Events
#include "../Events System/Event.hpp"
#include "../Events System/ApplicationEvent.hpp"

// Rendering
#include "../Rendering/LayerStacker.hpp"
//#include "../ImGui/ImGuiLayer.hpp"

#include <memory>

namespace krakoa
{
#if defined (_MSC_VER)
#	pragma warning(push)
#	pragma warning(disable:4251)
		EXPIMP_TEMPLATE template class KRAKOA_API std::unique_ptr<iWindow, std::default_delete<iWindow>>;

	class KRAKOA_API Application : public pattern::SimpleSingleton<Application>
	{
	public:
		Application(Token&);
		virtual ~Application();

		void Initialize();
		void Run();
		USE_RESULT constexpr bool IsRunning() const;
		virtual void Shutdown() {}

		iWindow& GetWindow() const;

	protected:
		void PushLayer(LayerBase* layer);
		void PushOverlay(LayerBase* overlay);

	private:
		void OnEvent(events::Event& e);
		bool OnWindowClosed(events::WindowClosedEvent& e);

	protected:
		bool isRunning;
		std::unique_ptr<iWindow> pWindow;
		//ImGuiLayer* pImGuiLayer;
		FPSCounter fpsCounter;
		LayerStacker layerStack;
	};

	Application* CreateApplication();

#	pragma warning(pop)
#endif
}