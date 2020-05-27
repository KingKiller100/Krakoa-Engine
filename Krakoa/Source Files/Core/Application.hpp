#pragma once

#include "../EngineConfig.hpp"

// Memory
#include "../Memory/MemoryTypes.hpp"

// Time Step
#include "../TimeStep.hpp"

// Window
#include "../iWindow.hpp"

// Pattern
#include "../Patterns/SimpleSingleton.hpp"

// Events
#include "../Events System/Event.hpp"
#include "../Events System/ApplicationEvent.hpp"

// Rendering
#include "../Layers/ImGui/ImGuiLayer.hpp"
#include "../Layers/LayerStacker.hpp"

#include "../Entity/EntityManager.hpp"

#include <memory>


namespace krakoa
{
	class KRAKOA_API Application : public patterns::SimpleSingleton<Application>
	{
	public:
		Application(Token&);
		virtual ~Application();

		virtual void Initialize();
		void Run() const;
		USE_RESULT bool IsRunning() const;
		virtual void ShutDown() = 0;

		iWindow& GetWindow() const;

	protected:
		void PushLayer(LayerBase* layer);
		void PushOverlay(LayerBase* overlay);
		void PopLayer(LayerBase* layer);
		void PopOverlay(LayerBase* overlay);

	private:
		void OnEvent(events::Event& e);
		bool OnWindowClosed(events::WindowClosedEvent& e);
		bool OnWindowResize(events::WindowResizeEvent& e) noexcept;

		static void RendererClear() noexcept;

	protected:
		bool isRunning;
		std::unique_ptr<iWindow> pWindow;
		LayerStacker layerStack;

		Solo_Ptr<EntityManager> entityManager;
		
	private:
		ImGuiLayer* pImGuiLayer;
		time::TimeStep timeStep;
		bool isMinimized;
	};

	void CreateApplication();
}
