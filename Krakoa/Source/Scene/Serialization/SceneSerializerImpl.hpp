#pragma once

#include <string>

namespace krakoa::scene
{
	class iScene;
}

namespace krakoa::scene::serialization::impl
{
	std::string SerializeScene(const iScene& scene);

	bool DeserializeScene(iScene& scene, const std::string& sceneData);
}
