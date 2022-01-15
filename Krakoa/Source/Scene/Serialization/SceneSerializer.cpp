#include "Precompile.hpp"
#include "SceneSerializer.hpp"

#include "../Entity/Entity.hpp"

#include "../../Debug/Debug.hpp"
#include "../../Debug/Instrumentor.hpp"
#include "../../Logging/EngineLogger.hpp"
#include "../../Util/Fmt.hpp"

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

		if (scene.expired())
		{
			KRK_ERR(util::Fmt("Attempting to serialize null scene", name));
			return;
		}
		const auto scn = scene.lock();
		
		const auto sceneData = impl::SerializeScene(*scn);

		auto outputPath = path;
		outputPath.replace_extension(s_FileExtension);
		klib::WriteFile(outputPath, sceneData);
		KRK_TRC("Serialization complete");
	}

	void SceneSerializer::SerializeBinary(const std::filesystem::path& path)
	{
		KRK_PROFILE_FUNCTION();
		throw klib::NotImplementedException{ "No implementation for " __FUNCTION__};
	}

	bool SceneSerializer::Deserialize(const std::filesystem::path& path)
	{
		KRK_PROFILE_FUNCTION();

		if (scene.expired())
		{
			KRK_ERR(util::Fmt("Attempting to serialize null scene", name));
			return false;;
		}

		
		std::ifstream file(path);
		std::stringstream stream;

		stream << file.rdbuf();

		if (const auto data = stream.str(); !data.empty())
		{
			const auto scn = scene.lock();
			scn->Clear();
			impl::DeserializeScene(*scn, data);
		}

		file.close();
		return true;
	}

	bool SceneSerializer::DeserializeBinary(const std::filesystem::path& path)
	{
		KRK_PROFILE_FUNCTION();
		throw klib::NotImplementedException{ "No implementation for " __FUNCTION__ };
		return false;
	}

	void SceneSerializer::SetScene(const Weak_Ptr<iScene> scene)
	{
		this->scene = scene;
	}
}
