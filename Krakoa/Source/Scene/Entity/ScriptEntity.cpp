#include "Precompile.hpp"
#include "ScriptEntity.hpp"

namespace krakoa::scene::ecs
{
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
}
