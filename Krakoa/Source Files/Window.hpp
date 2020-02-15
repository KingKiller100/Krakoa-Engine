#pragma once

#include <Maths/Vectors/Vector2.hpp>

#include <Core/EngineCore.hpp>
#include <Events System/Event.hpp>

namespace krakoa
{
	using namespace kMaths;
	using namespace events;

	struct WindowProperties
	{
		std::string title;
		Vector2u dimensions;

		WindowProperties(const std::string&& title = "Krakoa Engine",
			const Vector2u dimensions = Vector2u(1280, 720))
			: title(title), dimensions(dimensions)
		{}
	};

	class KRAKOA_API Window
	{
	public:
		using EventCallbackFunc = std::function<void(Event&)>;

		virtual ~Window() 
			= default;

		virtual void OnUpdate() noexcept = 0;

		virtual constexpr kMaths::Vector2u GetDimensions() const  noexcept = 0;
		virtual constexpr unsigned GetWidth() const noexcept = 0;
		virtual constexpr unsigned GetWidth() const noexcept = 0;

		virtual void SetEventCallback(const EventCallbackFunc& cb) = 0;
		virtual void SetVsync(bool isEnabled) = 0;
		virtual constexpr bool IsVsyncActive() const noexcept = 0;

		static std::unique_ptr<Window> Create(const WindowProperties& props = WindowProperties());
	};
}
