#include "Precompile.hpp"
#include "EntityComponentSystem.hpp"

#include "../../Debug/Instrumentor.hpp"
#include "../../Debug/Debug.hpp"
#include "../../Logging/MemoryLogger.hpp"


namespace krakoa
{
	EntityComponentSystem::EntityComponentSystem(Token)
		: nextFreeID(0)
	{}

	EntityComponentSystem::~EntityComponentSystem()
	{
		KRK_PROFILE_FUNCTION();

		RemoveAllEntities();
		/*const auto status = Entity::GetStatus();
		MEM_INF(status);
		Entity::TerminatePool();*/
	}

	void EntityComponentSystem::RemoveAllEntities() noexcept
	{
		KRK_PROFILE_FUNCTION();

		entities.clear();
		componentMap.clear();
		nextFreeID = 0;
	}

	EntityUID EntityComponentSystem::Add()
	{
		KRK_PROFILE_FUNCTION();

		const auto entity_uid = GenerateNewID();
		const auto& insertion_pair =
			entities.insert(
				std::make_pair(entity_uid, std::unordered_map<ComponentUID, ComponentWrapper*>())
			);
		return insertion_pair.first->first;
	}

	void EntityComponentSystem::RemoveEntity(EntityUID id)
	{
		KRK_PROFILE_FUNCTION();

		if (!HasEntity(id))
			return;

		const auto& entity = entities.at(id);

		for (auto& [compID, compVec] : componentMap)
		{
			size_t index = 0;

			for (auto&& component : compVec)
			{
				if (component.GetOwner() == id)
					break;
				++index;
			}

			if (index < compVec.size())
				compVec.erase(compVec.begin() + index);
		}

		entities.erase(id);
		nextFreeID = id;
	}

	bool EntityComponentSystem::HasEntity(EntityUID id) const
	{
		KRK_PROFILE_FUNCTION();

		const auto iter = entities.find(id);

		return iter != entities.end();
	}

	EntityUID EntityComponentSystem::GetEntity(EntityUID id) const
	{
		KRK_PROFILE_FUNCTION();

		const auto iter = entities.find(id);

		KRK_ASSERT(iter != entities.end(),
			klib::ToString("Entity ID \"{0}\" does not exist", id));

		return iter->first;
	}

	EntityUID EntityComponentSystem::GenerateNewID()
	{
		KRK_PROFILE_FUNCTION();

		for (const auto& entity : entities)
		{
			if (entity.first <= nextFreeID)
				++nextFreeID;
			else
				break;
		}
		return nextFreeID;
	}

	bool EntityComponentSystem::RemoveAllComponents(EntityUID id) noexcept
	{
		if (!HasEntity(id))
			return false;


		auto& entComps = entities.at(id);
		const auto ecSize = entComps.size();
		for (auto i = 0; i < ecSize; ++i)
		{
			const auto uid = entComps.begin()->second->GetUID();
			auto& compVec = componentMap.at(uid);

			const auto iter = std::find_if(compVec.begin(), compVec.end()
				, [&id](ComponentWrapper& comp)
				{
					return id == comp.GetOwner();
				});

			entComps.erase(uid);
			compVec.erase(iter);
		}

		return true;
	}

	// void EntityComponentSystem::Draw()
	// {
	// 	KRK_PROFILE_FUNCTION();
	//
	// 	for (auto& entity : entities)
	// 	{
	// 		if (!entity->HasComponent<components::Appearance2DComponent>()
	// 			|| !entity->HasComponent<components::TransformComponent>())
	// 			continue;
	//
	// 		const auto& appearance = entity->GetComponent<components::Appearance2DComponent>();
	// 		const auto& transform = entity->GetComponent<components::TransformComponent>();
	//
	// 		switch (appearance.GetGeometryType()) {
	// 		case graphics::GeometryType::QUAD:
	// 		{
	// 			graphics::Renderer2D::DrawQuad(appearance.GetSubTexture(),
	// 				transform.GetPosition(),
	// 				transform.GetScale(),
	// 				transform.GetRotation(),
	// 				appearance.GetColour(),
	// 				appearance.GetTilingFactor());
	// 		}
	// 		break;
	// 		case graphics::GeometryType::TRIANGLE:
	// 		{
	// 			graphics::Renderer2D::DrawTriangle(appearance.GetSubTexture(),
	// 				transform.GetPosition(),
	// 				transform.GetScale(),
	// 				transform.GetRotation(),
	// 				appearance.GetColour(),
	// 				appearance.GetTilingFactor());
	// 		}
	// 		break;
	// 		case graphics::GeometryType::CIRCLE:
	// 			/*	{
	// 					graphics::Renderer2D::DrawQuad(appearance.GetSubTexture(),
	// 						transform.GetPosition(),
	// 						transform.GetScale(),
	// 						transform.GetRotation(),
	// 						appearance.GetColour(),
	// 						appearance.GetTilingFactor());
	// 				}*/
	// 			break;
	// 		default: // case of an unknown geometry type
	// 			KRK_FATAL("Failed to draw unknown geometry type");
	// 			break;
	// 		}
	// 	}
	// 	graphics::Renderer2D::EndScene();
	// }
}
