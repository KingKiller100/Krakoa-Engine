#include "Precompile.hpp"
#include "SceneSerializer.hpp"

namespace krakoa::scene
{
	SceneSerializer::SceneSerializer(const Multi_Ptr<iScene>& scene)
		: scene(scene)
	{
		
	}

	void SceneSerializer::Serialize(const std::filesystem::path& path)
	{

		
	}

	void SceneSerializer::SerializeBinary(const std::filesystem::path& path)
	{
	}

	bool SceneSerializer::Deserialize(const std::filesystem::path& path)
	{
		return false;
	}

	bool SceneSerializer::DeserializeBinary(const std::filesystem::path& path)
	{
		return false;
	}
}
