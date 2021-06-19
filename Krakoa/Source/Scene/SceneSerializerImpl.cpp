#include "SceneSerializerImpl.hpp"

namespace YAML
{
	ostream_wrapper& operator<<(ostream_wrapper& stream, const std::string_view& str)
	{
		stream.write(str.data());
		return stream;
	}

	Emitter& operator<<(Emitter& emitter, const std::string_view& str)
	{
		return emitter.Write(str.data());
	}

	Emitter& operator<<(Emitter& emitter, const krakoa::scene::ecs::Entity& entity)
	{
		
	}
}
