#pragma once

#include "../../Instrumentor.hpp"
#include "../../EngineConfig.hpp"

namespace krakoa
{
	class Entity;
	
	class ComponentBase
	{
	public:
		virtual ~ComponentBase() noexcept = default;
		USE_RESULT virtual const char* GetType() const noexcept = 0;

		virtual void Initialize() {}
		virtual void Update(const double dt) {}

		USE_RESULT bool IsActive() const
		{
			return active;
		}

		void SetOwner(Entity* entity)
		{
			owner = entity;
		}
		
	protected:
		bool active = true;
		Entity* owner = nullptr;
	};

#define STATIC_GET_COMPONENT_TYPE(self) static const char* GetStaticType()\
	{\
	KRK_PROFILE_FUNCTION();\
	return typeid(self).name();\
	}\
	
}
