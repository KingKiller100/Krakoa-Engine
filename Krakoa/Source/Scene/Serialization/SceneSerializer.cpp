﻿#include "Precompile.hpp"
#include "SceneSerializer.hpp"

#include "../Entity/Entity.hpp"

#include "../../Debug/Debug.hpp"
#include "../../Debug/Instrumentor.hpp"
#include "../../Logging/EngineLogger.hpp"

#include "SceneSerializerImpl.hpp"

#include <Utility/Debug/Exceptions/NotImplementedException.hpp>
#include <Utility/FileSystem/kFileSystem.hpp>

namespace krakoa::scene::serialization
{
	SceneSerializer::SceneSerializer(const Multi_Ptr<iScene>& scene)
		: scene(scene)
		, name(scene->GetName())
	{
		KRK_PROFILE_FUNCTION();
	}

	void SceneSerializer::Serialize(const std::filesystem::path& path) const
	{
		KRK_PROFILE_FUNCTION();

		const auto sceneData = impl::SerializeScene(*scene);

		auto outputPath = path;
		outputPath /= name;
		outputPath.replace_extension(s_FileExtension);
		klib::WriteFile(outputPath, sceneData);
		KRK_TRC("Serialization complete");
	}

	void SceneSerializer::SerializeBinary(const std::filesystem::path& path)
	{
		KRK_PROFILE_FUNCTION();
	}

	bool SceneSerializer::Deserialize(const std::filesystem::path& path)
	{
		KRK_PROFILE_FUNCTION();
		auto inPath = path / name;
		inPath += s_FileExtension;
		std::ifstream file(inPath);
		std::stringstream stream;

		stream << file.rdbuf();

		if (const auto data = stream.str(); !data.empty())
		{
			scene->Clear();
			impl::DeserializeScene(*scene, data);
		}

		file.close();
		return true;
	}

	bool SceneSerializer::DeserializeBinary(const std::filesystem::path& path)
	{
		KRK_PROFILE_FUNCTION();
		throw klib::NotImplementedException{ __FUNCTION__ };
		return false;
	}
}
