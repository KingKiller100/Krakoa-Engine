#include "Precompile.hpp"
#include "Scene.hpp"

#include "Entity/Components/TagComponent.hpp"

namespace krakoa::scene
{
	Scene::Scene()
		: ecs(EntityComponentSystem::Pointer())
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::OnUpdate(float time)
	{
		auto entity = ecs->Add();
		ecs->RegisterComponent<TagComponent>(entity, std::string_view());
	}
}
