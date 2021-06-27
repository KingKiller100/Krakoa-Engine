#pragma once

#include "../iScene.hpp"
#include "../../Core/PointerTypes.hpp"

#include <filesystem>

namespace krakoa::scene::serialization
{
	class SceneSerializer
	{
		static constexpr auto s_FileExtension = ".yaml";
		
	public:
		SceneSerializer() = default;
		SceneSerializer(const Multi_Ptr<iScene>& scene);
		
		void Serialize(const std::filesystem::path& path) const;
		void SerializeBinary(const std::filesystem::path& path);
		bool Deserialize(const std::filesystem::path& path);
		bool DeserializeBinary(const std::filesystem::path& path);

		void SetScene(const Weak_Ptr<iScene> scene);

	private:
		Weak_Ptr<iScene> scene;
		std::string name;
	};
}

