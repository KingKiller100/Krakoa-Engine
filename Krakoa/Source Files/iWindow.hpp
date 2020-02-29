#pragma once

#include <Maths/Vectors/Vector2.hpp>

#include <Core/EngineCore.hpp>
#include <Events System/Event.hpp>

namespace krakoa
{
	using namespace kmaths;
	using namespace events;

	struct WindowProperties
	{
		std::string title;
		Vector2s dimensions;

		WindowProperties(const std::string&& title = "Krakoa Engine",
			const Vector2s dimensions = Vector2s(700, 400))
			: title(title), dimensions(dimensions)
		{}
	};

	// Interface class for OS platforms
	class KRAKOA_API iWindow
	{
	public:
		using EventCallbackFunc = std::function<void(Event&)>;

		virtual ~iWindow() 
			= default;

		virtual void OnUpdate() = 0;

		virtual kmaths::Vector2s& GetDimensions() = 0;
		virtual unsigned GetWidth() const = 0;
		virtual unsigned GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFunc& cb) = 0;
		virtual void SetVsync(bool isEnabled) = 0;
		virtual bool IsVsyncActive() const = 0;

		static iWindow* Create(const WindowProperties& props = WindowProperties());
	};
}
