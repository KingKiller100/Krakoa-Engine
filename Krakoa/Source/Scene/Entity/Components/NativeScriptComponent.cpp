#include "Precompile.hpp"
#include "NativeScriptComponent.hpp"


namespace krakoa::scene::ecs::components
{
	bool NativeScriptComponent::IsActive() const
	{
		return !scripts.empty() && scripts.begin()->second != nullptr;
	}

	void NativeScriptComponent::InvokeCreate(Entity* entity)
	{
		initScriptFunc();
		SetEntity(entity);

		for (auto& [id, script] : scripts)
			script->OnCreate();
	}

	void NativeScriptComponent::InvokeDestroy()
	{
		for (auto& [id, script] : scripts)
			script->OnDestroy();

		destroyScriptFunc();
	}

	void NativeScriptComponent::InvokeUpdate(float deltaTime)
	{
		for (auto& [id, script] : scripts)
			script->OnUpdate(deltaTime);
	}

	void NativeScriptComponent::SetEntity(Entity* entity)
	{
		for (auto& [id, script] : scripts)
			script->SetEntity(entity);
	}
}
