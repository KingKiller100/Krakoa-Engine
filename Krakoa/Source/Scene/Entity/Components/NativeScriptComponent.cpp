#include "Precompile.hpp"
#include "NativeScriptComponent.hpp"


namespace krakoa::scene::ecs::components
{
	bool NativeScriptComponent::IsActive() const
	{
		return owner != nullptr;
	}

	void NativeScriptComponent::InvokeCreate(Entity* entity)
	{
		initScriptFunc();
		SetEntity(entity);
		owner->OnCreate();
	}

	void NativeScriptComponent::InvokeDestroy()
	{
		owner->OnDestroy();
		destroyScriptFunc();
	}

	void NativeScriptComponent::InvokeUpdate(float deltaTime)
	{
		owner->OnUpdate(deltaTime);
	}

	void NativeScriptComponent::SetEntity(Entity* entity)
	{
		owner->SetEntity(entity);
	}
}
