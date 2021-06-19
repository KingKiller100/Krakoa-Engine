#pragma once

#include "iScene.hpp"
#include "../Core/PointerTypes.hpp"

#include <filesystem>

namespace krakoa::scene::serialize
{
	class SceneSerializer
	{
		static constexpr auto s_FileExtension = ".yaml";
		
	public:
		SceneSerializer(const Multi_Ptr<iScene>& scene);
		
		void Serialize(const std::filesystem::path& path);
		void SerializeBinary(const std::filesystem::path& path);
		bool Deserialize(const std::filesystem::path& path);
		bool DeserializeBinary(const std::filesystem::path& path);
		
	private:
		std::string name;
		Multi_Ptr<iScene> scene;
	};
}

