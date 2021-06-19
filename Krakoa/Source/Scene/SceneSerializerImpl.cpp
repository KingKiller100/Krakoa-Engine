#include "Precompile.hpp"
#include "SceneSerializerImpl.hpp"

#include "Entity/Entity.hpp"
#include "Entity/Components/TagComponent.hpp"
#include "Entity/Components/TransformComponent.hpp"

		using namespace krakoa::scene::ecs;
namespace YAML
{
	namespace 
	{
		Emitter& operator<<(Emitter& e, const components::TagComponent& tag)
		{
			e << tag.GetRawTag();
			return e;
		}

		ostream_wrapper& operator<<(ostream_wrapper& stream, const components::TagComponent& tag)
		{
			stream << tag.GetTag();
			return stream;
		}
		
		Emitter& operator<<(Emitter& e, const components::TransformComponent& transform)
		{
			// e << BeginMap;
			return e;
		}

		ostream_wrapper& operator<<(ostream_wrapper& stream, const components::TransformComponent& transform)
		{
			// stream << tag.GetTag();
			return stream;
		}
		
	}
	
	ostream_wrapper& operator<<(ostream_wrapper& stream, const std::string_view& str)
	{
		stream.write(str.data());
		return stream;
	}

	Emitter& operator<<(Emitter& emitter, const std::string_view& str)
	{
		return emitter.Write(str.data());
	}

	Emitter& operator<<(Emitter& emitter, const Entity& entity)
	{
		emitter << BeginMap;
		emitter << Key << "Entity" << Value << entity.GetID().GetValue();
		emitter << Key << "Tag" << Value << entity.GetComponent<components::TagComponent>();
		// emitter << Key << "Transform" << Value << entity.GetComponent<components::TransformComponent>();
		emitter << EndMap;

		return emitter;
	}
}
