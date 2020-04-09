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

// Camera
#include "../Camera/OrthographicCamera.hpp"

// Rendering
#include "../Rendering/LayerStacker.hpp"
#include "../ImGui/ImGuiLayer.hpp"
#include "../Rendering/Rendering Resources/iShader.hpp"
#include "../Rendering/Rendering Resources/iBuffers.hpp"
#include "../Rendering/Rendering Resources/iVertexArray.hpp"

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
		OrthographicCamera camera;

	private:
		// To go in Renderer
		std::unique_ptr<graphics::iShader> pTriangleShader;
		std::unique_ptr<graphics::iShader> pSquareShader;
		std::unique_ptr<graphics::iVertexArray> pTriangeVA;
		std::unique_ptr<graphics::iVertexArray> pSquareVA;
	};

	void CreateApplication();
}