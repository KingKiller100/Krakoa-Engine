#pragma once

#include "../../Debug/Instrumentor.hpp"
#include "../../Core/EngineMacros.hpp"

namespace krakoa
{
	class Entity;

	class ComponentBase
	{
	public:
		constexpr ComponentBase()
			: active(true)
			, owner(nullptr)
		{}

		virtual ~ComponentBase() noexcept = default;
		USE_RESULT virtual const char* GetType() const noexcept = 0;

		virtual void Initialize()
		{
			KRK_PROFILE_FUNCTION();
		}
		virtual void Update(const double dt)
		{
			KRK_PROFILE_FUNCTION();
		}

		USE_RESULT bool IsActive() const
		{
			KRK_PROFILE_FUNCTION();
			return active;
		}

		void SetOwner(Entity* entity)
		{
			KRK_PROFILE_FUNCTION();
			owner = entity;
		}

		constexpr ComponentBase(const ComponentBase&) = delete;
		constexpr ComponentBase& operator=(const ComponentBase&) = delete;
		
	protected:
		bool active;
		Entity* owner;
	};

#define STATIC_GET_COMPONENT_TYPE(self) static const char* GetStaticType()\
	{\
	KRK_PROFILE_FUNCTION();\
	return typeid(self).name();\
	}\

}
