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

#include "../Graphics/Framebuffers/iFrameBuffer.hpp"

#include "../Util/UniqueID.hpp"

#include "../Platform/OperatingSystem/iOperatingSystem.hpp"

#include <Template/kToImpl.hpp>

#include <memory>


namespace krakoa
{
	class KRAKOA_API Application : public patterns::ManagerBase<Application>, util::TypeUniqueIdentifier
	{
	public:
		Application( Token&, std::string_view appName = "Krakoa Engine" );
		virtual ~Application();

		virtual void Initialize();
		virtual void ShutDown();
		void Run() const;
		void Close() noexcept;
		USE_RESULT bool IsRunning() const;

		USE_RESULT iWindow& GetWindow() const;
		USE_RESULT gfx::iFrameBuffer& GetFrameBuffer() const;
		USE_RESULT ImGuiLayer& GetImGuiLayer() const;

		template <typename Manager>
		USE_RESULT Manager& GetManager() const
		{
			return klib::ToImpl<Manager>( managers[GetUniqueID<Manager>()] );
		}

	protected:
		void PushLayer( LayerBase* layer );
		void PushOverlay( LayerBase* overlay );
		void PopLayer( LayerBase* layer );
		void PopOverlay( LayerBase* overlay );

		template <typename Manager>
		void AddManager( Manager* ptr )
		{
			managers.emplace_back( ptr );
			( void )GetUniqueID<Manager>();
		}

		template <typename Manager, typename ...Args>
		void RegisterManager( Args&& ...args )
		{
			Manager::Create( std::forward<Args>( args )... );
			AddManager( Manager::Pointer() );
		}

	private:
		void PushInternalLayers();

		void OnEvent( events::Event& e );
		bool OnWindowClosed( events::WindowClosedEvent& e );
		bool OnWindowResize( const events::WindowResizeEvent& e ) noexcept;

	protected:
		bool isRunning;
		std::unique_ptr<iWindow> pWindow;
		LayerStacker layerStack;

		Solo_Ptr<gfx::iFrameBuffer> frameBuffer;

		std::vector<iSingleton*> managers;

	private:
		ImGuiLayer* pImGuiLayer;
		time::TimeStep timeStep;
		bool isMinimized;
	};

	Application& GetApp();

	iWindow& GetWindow();
}
