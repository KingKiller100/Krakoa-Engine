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
		const auto entity = ecs->Add();
		ecs->RegisterComponent<TagComponent>(entity, std::string_view());
		const auto compExists = ecs->HasComponent<TagComponent>();
		const auto enttCompExists = ecs->HasComponent<TagComponent>(entity);
		const auto& comp = ecs->GetComponent<TagComponent>(entity);
	}
}
