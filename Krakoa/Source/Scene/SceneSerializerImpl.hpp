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
	ostream_wrapper& operator<<(ostream_wrapper& stream,
	                                   const std::string_view& str);

	Emitter& operator<<(Emitter& emitter,
	                           const std::string_view& str);

	Emitter& operator<<(Emitter& emitter, 
		const krakoa::scene::ecs::Entity& entity);
}


namespace krakoa::scene::serialize
{
	using namespace YAML;
}
