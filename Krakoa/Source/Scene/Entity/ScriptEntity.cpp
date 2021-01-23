#include "Precompile.hpp"
#include "ScriptEntity.hpp"

namespace krakoa::scene::ecs
{
	ScriptEntity::~ScriptEntity()
		= default;

	EntityUID ScriptEntity::GetEntityID() const
	{
		return pEntity->GetID();
	}

	bool ScriptEntity::IsActive() const
	{
		return pEntity->IsActive();
	}

	void ScriptEntity::Deactivate() const
	{
		pEntity->Deactivate();
	}

	void ScriptEntity::Activate() const
	{
		pEntity->Activate();
	}

	void ScriptEntity::SetEntity(Entity* e)
	{
		pEntity = e;
	}

	void ScriptEntity::OnCreate()
	{}

	void ScriptEntity::OnDestroy()
	{}

	void ScriptEntity::OnUpdate(float deltaTime)
	{}
}
