#include "Precompile.hpp"
#include "ComponentWrapper.hpp"

namespace krakoa
{
	ComponentWrapper::ComponentWrapper(UID compUid, EntityUID entityUId)
		: active(true)
		, owner(entityUId)
		, uid(compUid)
	{	}

	ComponentWrapper::ComponentWrapper(ComponentWrapper&& other) noexcept
		: uid(std::move(other.GetUID()))
	{
		*this = std::move(other);
	}

	ComponentWrapper& ComponentWrapper::operator=(ComponentWrapper&& other) noexcept
	{
		if (this != &other)
		{
			component.swap(other.component);
			other.component.release();

			active = std::move(other.active);
			owner = std::move(other.owner);
		}

		return *this;
	}

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

	ComponentWrapper::UID ComponentWrapper::GetUID() const
	{
		return uid;
	}

	EntityUID ComponentWrapper::GetOwner() const
	{
		return owner;
	}

	void ComponentWrapper::SetOwner(EntityUID entity)
	{
		KRK_PROFILE_FUNCTION();
		owner = entity;
	}

	// void ComponentWrapper::ReleaseComponent()
	// {
	// 	KRK_PROFILE_FUNCTION();
	// 	if (component)
	// 	{
	// 		free(component);
	// 		component = nullptr;
	// 	}
	// }
}
