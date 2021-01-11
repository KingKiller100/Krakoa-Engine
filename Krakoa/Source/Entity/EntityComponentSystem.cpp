#include "Precompile.hpp"
#include "EntityComponentSystem.hpp"

#include "Entity.hpp"
#include "../Graphics/2D/Renderer2D.hpp"
#include "../Debug/Debug.hpp"

#include "../Logging/MemoryLogger.hpp"

#include "Components/TransformComponent.hpp"
#include "Components/AppearanceComponent.hpp"


namespace krakoa
{
	EntityComponentSystem::EntityComponentSystem(Token)
		: nextFreeID(0)
	{}

	EntityComponentSystem::~EntityComponentSystem()
	{
		const auto status = Entity::GetStatus();
		MEM_INF(status);
		RemoveAll();
		Entity::TerminatePool();
	}

	void EntityComponentSystem::RemoveAll() noexcept
	{
		nameMap.clear();
		componentMap.clear();
	}

	Entity& EntityComponentSystem::Add(const std::string& name)
	{
		if (HasName(name))
			return GetEntity(name);

		const auto entity_uid = GenerateNewID();
		AddName(name, entity_uid);

		const auto& entity =
			entities.emplace_back(Make_Solo<Entity>(name));
		SortEntities();
		return *entity;
	}

	void EntityComponentSystem::Remove(const std::string_view& name)
	{
		KRK_PROFILE_FUNCTION();

		entities.erase(std::find_if(entities.begin(), entities.end(), [&name](const Solo_Ptr<Entity>& entity)
			{
				return entity->GetName() == name;
			}));
	}

	void EntityComponentSystem::Remove(EntityComponentSystem::EntityUID id)
	{
		KRK_PROFILE_FUNCTION();

		entities.erase(std::find_if(entities.begin(), entities.end(), [id](const Solo_Ptr<Entity>& entity)
			{
				return entity->GetID() == id;
			}));
	}

	void EntityComponentSystem::Update(const float dt)
	{
		KRK_PROFILE_FUNCTION();

		for (auto& entity : entities)
		{
			if (!entity->IsActive())
				continue;

			entity->Update(dt);
		}
	}

	void EntityComponentSystem::Draw()
	{
		KRK_PROFILE_FUNCTION();

		for (auto& entity : entities)
		{
			if (!entity->HasComponent<components::Appearance2DComponent>()
				|| !entity->HasComponent<components::TransformComponent>())
				continue;

			const auto& appearance = entity->GetComponent<components::Appearance2DComponent>();
			const auto& transform = entity->GetComponent<components::TransformComponent>();

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

	bool EntityComponentSystem::Find(const std::string_view& name)
	{
		KRK_PROFILE_FUNCTION();

		const auto iter = std::find_if(entities.begin(), entities.end(), [&name](const Solo_Ptr<Entity>& entity)
			{
				return entity->GetName() == name;
			});

		return iter != entities.end();
	}

	bool EntityComponentSystem::Find(EntityComponentSystem::EntityUID id)
	{
		KRK_PROFILE_FUNCTION();

		const auto iter = std::find_if(entities.begin(), entities.end(), [&id](const Solo_Ptr<Entity>& entity)
			{
				return entity->GetID() == id;
			});

		return iter != entities.end();
	}

	Entity& EntityComponentSystem::GetEntity(const std::string_view& name) const
	{
		KRK_PROFILE_FUNCTION();

		const auto iter = std::find_if(entities.begin(), entities.end(), [&name](const Solo_Ptr<Entity>& entity)
			{
				return entity->GetName() == name;
			});

		KRK_ASSERT(iter != entities.end(),
			klib::ToString("Entity \"{0}\" does not exist", name));

		return **iter;
	}

	Entity& EntityComponentSystem::GetEntity(EntityComponentSystem::EntityUID id) const
	{
		KRK_PROFILE_FUNCTION();

		const auto iter = std::find_if(entities.begin(), entities.end(), [id](const Solo_Ptr<Entity>& entity)
			{
				return entity->GetID() == id;
			});

		KRK_ASSERT(iter != entities.end(),
			klib::ToString("Entity ID \"{0}\" does not exist", id));

		return **iter;
	}

	const std::vector<Solo_Ptr<Entity>>& EntityComponentSystem::GetEntities() const
	{
		return entities;
	}

	void EntityComponentSystem::SortEntities()
	{
		KRK_PROFILE_FUNCTION();

		if (entities.size() < 2)
			return;

		std::sort(entities.begin(), entities.end(), [](const Solo_Ptr<Entity>& e1, const Solo_Ptr<Entity>& e2)
			{
				return e1->IsActive() == true
					&& e2->IsActive() == false;
			});
	}

	EntityComponentSystem::EntityUID EntityComponentSystem::GenerateNewID()
	{
		for (const auto& entity : entities)
		{
			if (entity.first == nextFreeID)
				++nextFreeID;
			else
				break;
		}
		return nextFreeID;
	}

	void EntityComponentSystem::AddName(const std::string& name, EntityUID id)
	{
		nameMap[name] = { id, Make_Solo<Entity>() };
	}

	bool EntityComponentSystem::HasName(const std::string& name) const
	{
		return nameMap.find(name) != nameMap.end();
	}
}
