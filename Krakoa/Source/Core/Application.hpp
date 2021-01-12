#pragma once

#include "EngineMacros.hpp"

// Memory
#include "PointerTypes.hpp"

// Time Step
#include "TimeStep.hpp"

// Window
#include "iWindow.hpp"

// Pattern
#include "../Patterns/ManagerBase.hpp"

// Events
#include "../EventsSystem/Event.hpp"
#include "../EventsSystem/ApplicationEvent.hpp"

// Rendering
#include "../Layers/ImGui/ImGuiLayer.hpp"
#include "../Layers/LayerStacker.hpp"

#include "../Scene/Entity/EntityComponentSystem.hpp"

#include <memory>
#include "../Graphics/Framebuffers/iFrameBuffer.hpp"


namespace krakoa
{
	class KRAKOA_API Application : public patterns::ManagerBase<Application>
	{
	public:
		Application(Token&, const std::string_view& appName = "Krakoa Engine");
		virtual ~Application();

		virtual void Initialize();
		virtual void ShutDown();
		void Run() const;
		void Close() noexcept;
		USE_RESULT bool IsRunning() const;

		iWindow& GetWindow() const;

		Multi_Ptr<graphics::iFrameBuffer>& GetFrameBuffer();
		ImGuiLayer& GetImGuiLayer() const;
	protected:
		void PushLayer(LayerBase* layer);
		void PushOverlay(LayerBase* overlay);
		void PopLayer(LayerBase* layer);
		void PopOverlay(LayerBase* overlay);

	private:
		void OnEvent(events::Event& e);
		bool OnWindowClosed(events::WindowClosedEvent& e);
		bool OnWindowResize(events::WindowResizeEvent& e) noexcept;

	protected:
		bool isRunning;
		std::unique_ptr<iWindow> pWindow;
		LayerStacker layerStack;

		Solo_Ptr<EntityComponentSystem> entityManager;
		
		Multi_Ptr<graphics::iFrameBuffer> frameBuffer;

	private:
		ImGuiLayer* pImGuiLayer;
		time::TimeStep timeStep;
		bool isMinimized;
	};

	void CreateApplication();
	Application& GetApp();
	iWindow& GetWindow();
}
