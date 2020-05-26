#pragma once

#include <HelperMacros.hpp>

namespace krakoa
{
	class ComponentBase
	{
	public:
		virtual ~ComponentBase() noexcept = default;
		USE_RESULT virtual const char* GetType() const noexcept = 0;

		virtual void Initialize() {}
		virtual void Update(const double dt) {}
	};

#define STATIC_GET_COMPONENT_TYPE(self) static const char* GetStaticType() { return typeid(self).name(); }
}
