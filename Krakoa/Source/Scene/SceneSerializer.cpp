#include "Precompile.hpp"
#include "SceneSerializer.hpp"

#include "../Debug/Debug.hpp"
#include "../Debug/Instrumentor.hpp"
#include "../Logging/EngineLogger.hpp"

#include "SceneSerializerImpl.hpp"
#include <Utility/Debug/Exceptions/NotImplementedException.hpp>

#include "Entity/Entity.hpp"

namespace krakoa::scene
{
	SceneSerializer::SceneSerializer(const Multi_Ptr<iScene>& scene)
		: scene(scene)
	{
		KRK_PROFILE_FUNCTION();
	}

	void SceneSerializer::Serialize(const std::filesystem::path& path)
	{
		KRK_PROFILE_FUNCTION();

		const auto sceneName = scene->GetName();

		KRK_INF(util::Fmt("Serializing scene: {0}", sceneName));
		
		serialize::Emitter emitter;

		emitter << serialize::BeginMap;
		emitter << serialize::Key << "Scene" << serialize::Value << sceneName;
		emitter << serialize::Key << "Entities" << serialize::Value;
		emitter << serialize::BeginSeq;
		scene->ForEach([&emitter](const ecs::Entity& entity)
		{
			if (entity.GetID().IsNull())
				return;
			
			emitter << entity;
			
			if (!emitter.good())
			{
				KRK_ERR(util::Fmt("Failed to entity: {0}", entity.GetID()));
			}
		});
		emitter << serialize::EndSeq;
		emitter << serialize::EndMap;

		if (!emitter.good())
		{
			KRK_ERR("Failed to serialize scene");
		}

		std::ofstream file(path);
		file << emitter.c_str();
		KRK_INF("Serialization complete");
	}

	void SceneSerializer::SerializeBinary(const std::filesystem::path& path)
	{
		KRK_PROFILE_FUNCTION();
	}

	bool SceneSerializer::Deserialize(const std::filesystem::path& path)
	{
		KRK_PROFILE_FUNCTION();
		throw klib::NotImplementedException{ __FUNCTION__ };
		return false;
	}

	bool SceneSerializer::DeserializeBinary(const std::filesystem::path& path)
	{
		KRK_PROFILE_FUNCTION();
		throw klib::NotImplementedException{ __FUNCTION__ };
		return false;
	}
}
