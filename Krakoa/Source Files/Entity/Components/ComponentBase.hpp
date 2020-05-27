#pragma once

#include <HelperMacros.hpp>
#include "../../Instrumentor.hpp"

namespace krakoa
{
	class ComponentBase
	{
	public:
		virtual ~ComponentBase() noexcept = default;
		USE_RESULT virtual const char* GetType() const noexcept = 0;

		virtual void Initialize() {}
		virtual void Update(const double dt) {}

		bool IsActive() const { return active; }
		
	protected:
		bool active = true;
	};

#define STATIC_GET_COMPONENT_TYPE(self) static const char* GetStaticType()\
	{\
	KRK_PROFILE_FUNCTION();\
	return typeid(self).name();\
	}\
	
}
