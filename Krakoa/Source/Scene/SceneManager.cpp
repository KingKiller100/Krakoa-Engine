#include "Precompile.hpp"
#include "SceneManager.hpp"

#include "Scene.hpp"

#include "../Graphics/2D/Renderer2D.hpp"


#include "Entity/Components/AppearanceComponent.hpp"
#include "Entity/Components/TransformComponent.hpp"

namespace krakoa::scene
{
	SceneManager::SceneManager()
		: entityComponentSystem(new EntityComponentSystem())
	{
	}

	SceneManager::~SceneManager()
	{
		RemoveAll();
		entityComponentSystem->RemoveAllEntities();
	}

	void SceneManager::Add(const std::string_view& name)
	{
		auto* scene = new Scene(name, entityComponentSystem);
		scenes[name.data()].reset(scene);
		currentScene = name;
		scene->OnLoad();
	}

	bool SceneManager::Remove(const std::string_view& name)
	{
		auto iter = scenes.find(name.data());
		if (iter == scenes.end())
			return false;

		scenes.erase(iter);
		return true;
	}

	void SceneManager::RemoveAll()
	{
		scenes.clear();
	}

	iScene& SceneManager::GetCurrentScene()
	{
		return *scenes.at(currentScene);
	}

	void SceneManager::LoadFromFile(const std::filesystem::path& path)
	{
	}

	void SceneManager::OnUpdate(const float deltaTime)
	{
		auto& scene = GetCurrentScene();
		scene.OnUpdate(deltaTime);
		DrawEntities(scene);
	}

	void SceneManager::DrawEntities(const iScene& scene) const
	{
		const auto list
			= entityComponentSystem->GetEntitiesWithComponents<components::Appearance2DComponent, components::TransformComponent>();

		for (const auto id : list)
		{
			const auto& entity = scene.GetEntity(id);

			const auto& appearance = entity.GetComponent<components::Appearance2DComponent>();
			const auto& transform = entity.GetComponent<components::TransformComponent>();

			switch (appearance.GetGeometryType()) {
			case graphics::GeometryType::QUAD:
				graphics::Renderer2D::DrawQuad(appearance, transform);
				break;
				
			case graphics::GeometryType::TRIANGLE:
				graphics::Renderer2D::DrawTriangle(appearance, transform);
				break;
				
			case graphics::GeometryType::CIRCLE:
				//	graphics::Renderer2D::DrawCircle(appearance, transform);
				break;
				
			default: // case of an unknown geometry type
				KRK_FATAL("Failed to draw unknown geometry type");
				break;
			}
		}
		
		graphics::Renderer2D::EndScene();
	}
}
