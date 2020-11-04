#include "Precompile.hpp"
#include "EntityManager.hpp"

#include "Entity.hpp"
#include "../Graphics/Renderer2D.hpp"

#include "Components/Transform.hpp"
#include "Components/Appearance.hpp"

namespace krakoa
{
	EntityManager::EntityManager(Token)
		: entities()
	{}

	EntityManager::~EntityManager()
	{
		const auto status = Entity::GetStatus();
		RemoveAll();
		Entity::TerminatePool();
	}

	void EntityManager::RemoveAll() noexcept
	{
		entities.clear();
	}

	Entity& EntityManager::Add()
	{
		entities.emplace_back(Make_Solo<Entity>());
		const auto& entity = entities.back();
		SortEntities();
		return *entity;
	}

	Entity& EntityManager::Add(const std::string_view& name)
	{
		entities.emplace_back(Make_Solo<Entity>(name));
		const auto& entity = entities.back();
		SortEntities();
		return *entity;
	}

	void EntityManager::Remove(const std::string_view& name)
	{
		KRK_PROFILE_FUNCTION();

		entities.erase(std::find_if(entities.begin(), entities.end(), [&name](const Solo_Ptr<Entity>& entity)
		{
			return entity->GetName() == name;
		}));
	}

	void EntityManager::Remove(const unsigned id)
	{
		KRK_PROFILE_FUNCTION();

		entities.erase(std::find_if(entities.begin(), entities.end(), [id](const Solo_Ptr<Entity>& entity)
		{
			return entity->GetID() == id;
		}));
	}

	void EntityManager::Update(const float dt)
	{
		KRK_PROFILE_FUNCTION();

		for (auto& entity : entities)
		{
			if (!entity->IsActive())
				continue;

			entity->Update(dt);
		}
	}

	void EntityManager::Draw()
	{
		KRK_PROFILE_FUNCTION();

		for (auto& entity : entities)
		{
			if (!entity->HasComponent<components::Appearance2D>()
				|| !entity->HasComponent<components::Transform>())
				continue;

			const auto& appearance = entity->GetComponent<components::Appearance2D>();
			const auto& transform = entity->GetComponent<components::Transform>();

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
				KRK_ASSERT(false, "Failed to draw unknown geometry type");
				break;
			}
		}
		graphics::Renderer2D::EndScene();
	}

	bool EntityManager::Find(const std::string_view& name)
	{
		KRK_PROFILE_FUNCTION();

		const auto iter = std::find_if(entities.begin(), entities.end(), [&name](const Solo_Ptr<Entity>& entity)
		{
			return entity->GetName() == name;
		});

		return iter != entities.end();
	}

	bool EntityManager::Find(const unsigned id)
	{
		KRK_PROFILE_FUNCTION();

		const auto iter = std::find_if(entities.begin(), entities.end(), [&id](const Solo_Ptr<Entity>& entity)
		{
			return entity->GetID() == id;
		});

		return iter != entities.end();
	}

	Entity& EntityManager::GetEntity(const std::string_view& name)
	{
		KRK_PROFILE_FUNCTION();

		const auto iter = std::find_if(entities.begin(), entities.end(), [&name](const Solo_Ptr<Entity>& entity)
		{
			return entity->GetName() == name;
		});

		if (iter == entities.end())
			KRK_ASSERT(false, klib::kString::ToString("Entity \"{0}\" does not exist", name));

		return **iter;
	}

	Entity& EntityManager::GetEntity(const unsigned id)
	{
		KRK_PROFILE_FUNCTION();

		const auto iter = std::find_if(entities.begin(), entities.end(), [id](const Solo_Ptr<Entity>& entity)
		{
			return entity->GetID() == id;
		});

		if (iter == entities.end())
			KRK_ASSERT(false, klib::kString::ToString("Entity ID \"{0}\" does not exist", id));

		return **iter;
	}

	const std::vector<Solo_Ptr<Entity>>& EntityManager::GetEntities() const
	{
		return entities;
	}

	void EntityManager::SortEntities()
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
}
