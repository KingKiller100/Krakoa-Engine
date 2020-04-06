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
#include "../ImGui/ImGuiLayer.hpp"

#include <memory>

namespace krakoa
{
	class KRAKOA_API Application : public pattern::SimpleSingleton<Application>
	{
	public:
		Application(Token&);
		virtual ~Application();

		void Initialize();
		void Run();
		USE_RESULT bool IsRunning() const;
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
		ImGuiLayer* pImGuiLayer;
		FPSCounter fpsCounter;
		LayerStacker layerStack;

	private:
		unsigned vertexArray, vertexBuffer, indexBuffer;
	};

	void CreateApplication();
}