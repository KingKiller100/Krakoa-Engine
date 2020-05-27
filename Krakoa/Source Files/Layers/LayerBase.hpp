#pragma once

#include "../EngineConfig.hpp"
#include "../Events System/Event.hpp"

namespace krakoa
{
	class KRAKOA_API LayerBase
	{
	public:
		LayerBase(const char* name);
		virtual ~LayerBase();

		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnUpdate(float deltaTime) {};
		virtual void OnRender() {};
		virtual void OnEvent(events::Event& e) {};

		constexpr const char* GetName() const;

	protected:
		const char* debugName;
		bool enabled;
	};
}

