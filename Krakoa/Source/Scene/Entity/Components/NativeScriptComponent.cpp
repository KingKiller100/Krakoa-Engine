#include "Precompile.hpp"
#include "NativeScriptComponent.hpp"


namespace krakoa::scene::ecs::components
{
	bool NativeScriptComponent::IsActive() const
	{
		return owner != nullptr;
	}

	void NativeScriptComponent::InvokeCreate()
	{
		ctorFunc();
		onCreateFunc(owner);
	}

	void NativeScriptComponent::InvokeDestroy()
	{
		onDestroyFunc(owner);
		dtorFunc();
	}

	void NativeScriptComponent::InvokeUpdate(float deltaTime)
	{
		onUpdateFunc(owner, deltaTime);
	}
}
