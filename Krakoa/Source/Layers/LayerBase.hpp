#pragma once

#include "../Core/EngineMacros.hpp"
#include "../EventsSystem/Event.hpp"

namespace krakoa
{
	class Application;

	class KRAKOA_API LayerBase
	{
	public:
		LayerBase(const char* name);
		virtual ~LayerBase();

		void SetApp(std::shared_ptr<Application> app);
		Application& GetApp() const;

		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnUpdate(float deltaTime) {};
		virtual void OnRender() {};
		virtual void OnEvent(events::Event& e) {};

		constexpr std::string_view GetName() const;

	protected:
		const char* debugName;
		bool enabled;
		std::weak_ptr<Application> app_;
	};
}

