#include "Precompile.hpp"
#include "SceneSerializerImpl.hpp"

#include "Entity/Entity.hpp"
#include "Entity/Components/TagComponent.hpp"
#include "Entity/Components/CameraComponent.hpp"
#include "Entity/Components/TransformComponent.hpp"
#include "Entity/Components/AppearanceComponent.hpp"
#include "Entity/Components/NativeScriptComponent.hpp"

#include "../Maths/Maths.hpp"

using namespace krakoa::scene::ecs;
namespace YAML
{
	namespace
	{
		using namespace components;
		
		Emitter& operator<<(Emitter& e, const maths::Vector3f& v)
		{
			e << BeginMap;
			e << Key << "X" << Value << v.x;
			e << Key << "Y" << Value << v.y;
			e << Key << "Z" << Value << v.z;
			e << EndMap;
			return e;
		}
		
		Emitter& operator<<(Emitter& e, const TagComponent& tag)
		{
			e << tag.GetRawTag();
			return e;
		}

		Emitter& operator<<(Emitter& e, const TransformComponent& transform)
		{
			const auto pos = transform.GetPosition();
			const auto rotation = transform.GetRotation();
			const auto scale = transform.GetScale();

			e << BeginMap;
			e << Key << "Position" << Value << pos;
			e << Key << "Rotation" << Value << rotation;
			e << Key << "Scale" << Value << scale;
			e << EndMap;
			return e;
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
		emitter << Key << "Components" << Value;
		emitter << BeginSeq;
		if (entity.HasComponent<TagComponent>())
			emitter << Key << "Tag" << Value << entity.GetComponent<TagComponent>();
		if (entity.HasComponent<TransformComponent>())
			emitter << Key << "Transform" << Value << entity.GetComponent<TransformComponent>();
		// if (entity.HasComponent<Appearance2DComponent>())
			// emitter << Key << "Appearance" << Value << entity.GetComponent<Appearance2DComponent>();
		// if (entity.HasComponent<CameraComponent>())
			// emitter << Key << "Camera" << Value << entity.GetComponent<CameraComponent>();
		// if (entity.HasComponent<NativeScriptComponent>())
			// emitter << Key << "Script" << Value << entity.GetComponent<NativeScriptComponent>().;
		emitter << EndSeq;
		emitter << EndMap;

		return emitter;
	}
}
