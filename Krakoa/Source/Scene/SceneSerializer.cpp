#include "Precompile.hpp"
#include "SceneSerializer.hpp"

#include "../Debug/Debug.hpp"
#include "../Debug/Instrumentor.hpp"
#include "../Logging/EngineLogger.hpp"

#include "SceneSerializerImpl.hpp"
#include <Utility/Debug/Exceptions/NotImplementedException.hpp>

#include "Entity/Entity.hpp"

#include <Utility/FileSystem/kFileSystem.hpp>

namespace krakoa::scene::serialize
{
	SceneSerializer::SceneSerializer(const Multi_Ptr<iScene>& scene)
		: scene(scene)
	{
		KRK_PROFILE_FUNCTION();
	}

	void SceneSerializer::Serialize(const std::filesystem::path& path)
	{
		KRK_PROFILE_FUNCTION();

		const auto sceneName = std::string(scene->GetName());

		KRK_INF(util::Fmt("Serializing scene: {0}", sceneName));
		
		impl::Emitter emitter;

		emitter << impl::BeginMap;
		emitter << impl::Key << "Scene" << impl::Value << sceneName;
		emitter << impl::Key << "Entities" << impl::Value;
		emitter << impl::BeginSeq;
		scene->ForEach([&emitter](const ecs::Entity& entity)
		{
			if (entity.GetID().IsNull())
				return;
			
			emitter << entity;
			
			if (!emitter.good())
			{
				KRK_ERR(util::Fmt("Failed to serialize entity: {0}\n"
					"Reason: {1}", entity.GetID(), emitter.GetLastError()));
			}
		});
		emitter << impl::EndSeq;
		emitter << impl::EndMap;

		if (!emitter.good())
		{
			KRK_ERR(util::Fmt("Failed to serialize scene: {0}", emitter.GetLastError()));
		}

		auto outputPath = path;
		outputPath /= sceneName;
		outputPath.replace_extension(s_FileExtension);
		klib::WriteFile(outputPath, emitter.c_str());
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
