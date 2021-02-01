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

#include "../Scene/SceneManager.hpp"

#include "../Graphics/Framebuffers/iFrameBuffer.hpp"

#include <memory>


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
		graphics::iFrameBuffer& GetFrameBuffer() const;
		ImGuiLayer& GetImGuiLayer() const;
		scene::SceneManager& GetSceneManager() const;
		
	protected:
		void PushLayer(LayerBase* layer);
		void PushOverlay(LayerBase* overlay);
		void PopLayer(LayerBase* layer);
		void PopOverlay(LayerBase* overlay);

		template<typename Manager>
		void RegisterSingleton()
		{
			
		}
		
	private:
		void OnEvent(events::Event& e);
		bool OnWindowClosed(events::WindowClosedEvent& e);
		bool OnWindowResize(events::WindowResizeEvent& e) noexcept;

		
	protected:
		bool isRunning;
		std::unique_ptr<iWindow> pWindow;
		LayerStacker layerStack;

		Solo_Ptr<scene::SceneManager> sceneManager;
		
		Solo_Ptr<graphics::iFrameBuffer> frameBuffer;

		std::vector<Solo_Ptr<patterns::iSingleton>> singletons;
		
	private:
		ImGuiLayer* pImGuiLayer;
		time::TimeStep timeStep;
		bool isMinimized;
	};

	void CreateApplication();
	Application& GetApp();
	iWindow& GetWindow();
}
