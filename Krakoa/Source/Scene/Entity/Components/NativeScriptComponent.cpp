#include "Precompile.hpp"
#include "NativeScriptComponent.hpp"


namespace krakoa::scene::ecs::components
{
	bool NativeScriptComponent::IsActive() const
	{
		if (scripts.empty())
			return false;

		for (auto& script : scripts)
		{
			if (script.second == nullptr)
				return false;
		}

		return true;
	}

	void NativeScriptComponent::InvokeCreate(Entity* entity)
	{
		for (const auto& func : initScriptFuncs)
			func();

		SetEntity(entity);

		for (const auto& [id, script] : scripts)
			script->OnCreate();
	}

	void NativeScriptComponent::InvokeDestroy()
	{
		for (auto& [id, script] : scripts)
		{
			script->OnDestroy();
		}

		for (auto& [id, script] : scripts)
		{
			script.reset();
		}
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
