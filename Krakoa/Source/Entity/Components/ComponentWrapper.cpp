#include "Precompile.hpp"
#include "ComponentWrapper.hpp"


namespace krakoa
{
	ComponentWrapper::~ComponentWrapper() noexcept = default;

	bool ComponentWrapper::IsActive() const
	{
		return active;
	}

	void ComponentWrapper::Activate() noexcept
	{
		active = true;
	}

	void ComponentWrapper::Deactivate() noexcept
	{
		active = false;
	}

	void ComponentWrapper::SetOwner(EntityUID entity)
	{
		KRK_PROFILE_FUNCTION();
		owner = entity;
	}
}
