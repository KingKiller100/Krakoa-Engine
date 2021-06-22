#pragma once

#include "EngineMacros.hpp"
#include "../EventsSystem/Event.hpp"

#include <Maths/Vectors/Vector2.hpp>

#include <any>

namespace krakoa
{
	using namespace kmaths;

	struct WindowProperties
	{
		std::string title;
		Vector2u dimensions;

		WindowProperties(const std::string_view& title,
			const Vector2u& dimensions = Vector2u(1520, 640))
			: title(title), dimensions(dimensions)
		{}
	};

	// Interface class for OS platforms
	class KRAKOA_API iWindow
	{
	public:
		using EventCallbackFunc = std::function<void(events::Event&)>;

		virtual ~iWindow() 
			= default;

		virtual void OnUpdate() = 0;

		virtual const kmaths::Vector2u& GetDimensions() const = 0;
		USE_RESULT virtual unsigned GetWidth() const = 0;
		USE_RESULT virtual unsigned GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFunc& cb) = 0;
		virtual void SetVsync(bool isEnabled) = 0;
		USE_RESULT virtual bool IsVsyncActive() const = 0;

		template<typename T>
		USE_RESULT T* GetNativeWindow() const noexcept
		{
			return std::any_cast<T*>(GetNativeWindow());
		}
		
		USE_RESULT virtual std::any GetNativeWindow() const noexcept = 0;

		static iWindow* Create(const WindowProperties& props);
	};
}
