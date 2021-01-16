#include "Precompile.hpp"
#include "ComponentWrapper.hpp"

#include "../../../Debug/Instrumentor.hpp"

namespace krakoa::scene::ecs
{
	ComponentWrapperBase::ComponentWrapperBase(UID compUid, EntityUID entityUId)
		: active(true)
		, owner(entityUId)
		, uid(compUid)
	{	}

	ComponentWrapperBase::ComponentWrapperBase(ComponentWrapperBase&& other) noexcept
		: uid(other.GetUID())
	{
		*this = std::move(other);
	}

	ComponentWrapperBase& ComponentWrapperBase::operator=(ComponentWrapperBase&& other) noexcept
	{
		if (this != &other)
		{
			active = std::move(other.active);
			owner = std::move(other.owner);
		}

		return *this;
	}

	ComponentWrapperBase::~ComponentWrapperBase() noexcept = default;

	bool ComponentWrapperBase::IsActive() const
	{
		return active;
	}

	void ComponentWrapperBase::Activate() noexcept
	{
		active = true;
	}

	void ComponentWrapperBase::Deactivate() noexcept
	{
		active = false;
	}

	ComponentWrapperBase::UID ComponentWrapperBase::GetUID() const
	{
		return uid;
	}

	EntityUID ComponentWrapperBase::GetOwner() const
	{
		return owner;
	}

	void ComponentWrapperBase::SetOwner(EntityUID entity)
	{
		KRK_PROFILE_FUNCTION();
		owner = entity;
	}
}
