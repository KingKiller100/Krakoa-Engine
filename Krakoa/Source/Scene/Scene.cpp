#include "Precompile.hpp"
#include "Scene.hpp"

#include "../Graphics/2D/Renderer2D.hpp"
#include "../Graphics/2D/Textures/SubTexture2d.hpp"

#include "Entity/Components/TagComponent.hpp"
#include "Entity/Components/TransformComponent.hpp"
#include "Entity/Components/AppearanceComponent.hpp"

namespace krakoa::scene
{
	Scene::Scene(const std::string_view& name)
		: name(name)
		, ecs(EntityComponentSystem::Pointer())
	{
	}

	Scene::~Scene()
	{
	}

	Entity& Scene::AddEntity(const std::string& name)
	{
		auto& entity = entities[name];
		entity.AddComponent<TagComponent>(name);
		return entity;
	}

	Entity& Scene::GetEntity(const std::string& name)
	{
		return entities.at(name);
	}

	Entity& Scene::GetEntity(EntityUID id)
	{
		auto iter(std::find_if(entities.begin(), entities.end(),
			[id](const decltype(entities)::value_type& pair)
			{
				return pair.second.GetID() == id;
			})
		);
		
		return iter->second;	
	}

	bool Scene::RemoveEntity(const std::string& name)
	{
		const auto iter = entities.find(name);
		if (iter == entities.end())
			return false;

		ecs->RemoveEntity(iter->second.GetID());
		entities.erase(iter);
		return true;
	}

	bool Scene::RemoveEntity(const Entity& entity)
	{
		ecs->RemoveEntity(entity.GetID());
		entities.erase(std::find_if(entities.begin(), entities.end()
			, [entity](const decltype(entities)::value_type& pair)
			{
				return pair.second == entity;
			}));
		return true;
	}


	void Scene::OnUpdate(float time)
	{
		Draw();
		// ecs->RegisterComponent<TagComponent>(entity, std::string_view());
		// const auto compExists = ecs->HasComponent<TagComponent>();
		// const auto enttCompExists = ecs->HasComponent<TagComponent>(entity);
		// const auto& comp = ecs->GetComponent<TagComponent>(entity);
	}

	void Scene::Draw()
	{
		KRK_PROFILE_FUNCTION();

		const auto list
			= ecs->GetEntitiesWithComponents<components::Appearance2DComponent, components::TransformComponent>();

		for (const auto id : list)
		{
			const auto& entity = GetEntity(id);

			const auto& appearance = entity.GetComponent<components::Appearance2DComponent>();
			const auto& transform = entity.GetComponent<components::TransformComponent>();

			switch (appearance.GetGeometryType()) {
			case graphics::GeometryType::QUAD:
			{
				graphics::Renderer2D::DrawQuad(appearance.GetSubTexture(),
					transform.GetPosition(),
					transform.GetScale(),
					transform.GetRotation(),
					appearance.GetColour(),
					appearance.GetTilingFactor());
			}
			break;
			case graphics::GeometryType::TRIANGLE:
			{
				graphics::Renderer2D::DrawTriangle(appearance.GetSubTexture(),
					transform.GetPosition(),
					transform.GetScale(),
					transform.GetRotation(),
					appearance.GetColour(),
					appearance.GetTilingFactor());
			}
			break;
			case graphics::GeometryType::CIRCLE:
				/*	{
						graphics::Renderer2D::DrawQuad(appearance.GetSubTexture(),
							transform.GetPosition(),
							transform.GetScale(),
							transform.GetRotation(),
							appearance.GetColour(),
							appearance.GetTilingFactor());
					}*/
				break;
			default: // case of an unknown geometry type
				KRK_FATAL("Failed to draw unknown geometry type");
				break;
			}
		}
		graphics::Renderer2D::EndScene();
	}
}
