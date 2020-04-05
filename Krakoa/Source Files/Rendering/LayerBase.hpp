#pragma once

#include "../Core/EngineCore.hpp"
#include "../Events System/Event.hpp"

namespace krakoa
{
	class KRAKOA_API LayerBase
	{
	public:
		LayerBase(const char* name);
		virtual ~LayerBase();

		virtual void OnAttach() = 0;
		virtual void OnDetach() = 0;
		virtual void OnUpdate() = 0;
		virtual void OnRender() = 0;
		virtual void OnEvent(events::Event& e) = 0;

		constexpr const char* GetName() const;

	protected:
		const char* debugName;
		bool enabled;
	};
}

