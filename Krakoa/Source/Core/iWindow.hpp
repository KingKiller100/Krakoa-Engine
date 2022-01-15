#pragma once

#include "EngineMacros.hpp"
#include "../EventsSystem/Event.hpp"

#include "../Maths/Maths.hpp"

#include <any>

namespace krakoa
{
	using namespace kmaths;

	struct WindowProperties
	{
		std::string title;
		maths::uSize dimensions;

		WindowProperties(std::string_view title,
			const maths::uSize& dimensions = maths::uSize(1520, 640))
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

		[[nodiscard]] virtual maths::uSize GetDimensions() const = 0;
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
