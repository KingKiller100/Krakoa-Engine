#pragma once

#include <yaml-cpp/yaml.h>

namespace krakoa {
	namespace scene {
		namespace ecs {
			class Entity;
		}
	}
}

namespace YAML
{
	Emitter& operator<<(Emitter& emitter,
	                           const std::string_view& str);

	Emitter& operator<<(Emitter& emitter, 
		const krakoa::scene::ecs::Entity& entity);
}


namespace krakoa::scene::serialize::impl
{
	using namespace YAML;

	
}
