#include "Precompile.hpp"
#include "SceneSerializerImpl.hpp"

#include "SceneSerializerKeys.hpp"

#include "../iScene.hpp"

#include "../Entity/Entity.hpp"
#include "../Entity/Components/TagComponent.hpp"
#include "../Entity/Components/CameraComponent.hpp"
#include "../Entity/Components/TransformComponent.hpp"
#include "../Entity/Components/AppearanceComponent.hpp"
#include "../Entity/Components/NativeScriptComponent.hpp"

#include "../../Graphics/Colour.hpp"
#include "../../Maths/Maths.hpp"
#include "../../Camera/SceneCamera.hpp"

#include <yaml-cpp/yaml.h>

// Third Party
using namespace YAML;

using namespace krakoa;
using namespace scene;
using namespace serialization;
using namespace impl;
using namespace ecs;
using namespace components;

namespace YAML
{
	template<typename T, maths::Length_t N>
	struct convert<maths::Vector<T, N>>
	{
		using Value_t = maths::Vector<T, N>;
		static Node encode(const Value_t& vec)
		{
			Node node;
			for (auto i = 0; i < N; ++i)
			{
				node.push_back(vec[i]);
			}
			return node;
		}

		static bool decode(const Node& node, Value_t& vec)
		{
			if (!node.IsSequence() || node.size() < N)
				return false;

			for (auto i = 0; i < N; ++i)
			{
				vec[i] = node[i].as<T>();
			}

			return true;
		}
	};

	template<>
	struct convert<gfx::Colour>
	{
		using Value_t = gfx::Colour;
		static Node encode(const Value_t& c)
		{
			Node node;
			const auto vec = c.ToFloats();
			for (auto i = 0; i < gfx::Colour::Length; ++i)
			{
				node.push_back(vec[i]);
			}
			return node;
		}

		static bool decode(const Node& node, Value_t& c)
		{
			if (!node.IsSequence() || node.size() < gfx::Colour::Length)
				return false;

			maths::Vector4f vec;
			for (auto i = 0; i < gfx::Colour::Length; ++i)
			{
				vec[i] = node[i].as<float>();
			}

			c = gfx::Colour(vec[0], vec[1], vec[2], vec[3]);

			return true;
		}
	};

	template<>
	struct convert<gfx::AssetID>
	{
		using Value_t = gfx::AssetID;
		static Node encode(const Value_t& aID)
		{
			Node node;
			node.push_back(aID.GetID());
			return node;
		}

		static bool decode(const Node& node, Value_t& aID)
		{
			aID = node.as<std::uint32_t>();
			return true;
		}
	};
}

namespace
{
	template<typename T, maths::Length_t N>
	Emitter& operator<<(Emitter& e, const maths::Vector<T, N>& v)
	{
		e << Flow;
		e << BeginSeq;
		for (auto i = 0; i < v.GetLength(); ++i)
		{
			e << v[i];
		}
		e << EndSeq;
		return e;
	}

	Emitter& operator<<(Emitter& e, const gfx::Colour& c)
	{
		const auto cFloats = c.GetRGBA();
		e << cFloats;
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
		e << Key << keys::TransformKeys::PositionKey << Value << pos;
		e << Key << keys::TransformKeys::RotationKey << Value << rotation;
		e << Key << keys::TransformKeys::ScaleKey << Value << scale;
		e << EndMap;
		return e;
	}

	Emitter& operator<<(Emitter& e, const Appearance2DComponent& appearance)
	{
		const auto subTexture = appearance.GetSubTexture();
		e << BeginMap;
		e << Key << keys::AppearanceKeys::GeometryTypeKey << Value << appearance.GetGeometryType().ToUnderlying();
		e << Key << keys::AppearanceKeys::ColourKey << Value << appearance.GetColour();
		e << Key << keys::AppearanceKeys::TilingFactorKey << Value << appearance.GetTilingFactor();
		e << Key << keys::AppearanceKeys::TextureIDKey << Value;

		if (!subTexture.GetTexture())
			e << "N/A";
		else
		{
			const auto& texCoordData = subTexture.GetTexCoordData();

			e << subTexture.GetTexture()->GetAssetID().GetID();
			e << Key << keys::AppearanceKeys::TexCoordKey << Value << *subTexture.GetTexCoord();
			e << Key << keys::AppearanceKeys::TexCoordDataKey;
			e << BeginMap;
			e << Key << keys::AppearanceKeys::BaseCoordsKey << Value;
			e << BeginSeq;
			for (auto&& coord : texCoordData.baseCoords)
			{
				e << coord;
			}
			e << EndSeq;
			e << Key << keys::AppearanceKeys::CoordIndexKey << Value << texCoordData.coordIndex;
			e << Key << keys::AppearanceKeys::SpriteDimensionsKey << Value << texCoordData.spriteDimensions;
			e << EndMap;
		}
		e << EndMap;
		return e;
	}

	Emitter& operator<<(Emitter& e, const CameraComponent& camera)
	{
		const auto& impl = camera.GetCamera<SceneCamera>();

		e << BeginMap;

		e << Key << keys::CameraKeys::IsPrimaryKey << camera.IsPrimary();
		e << Key << keys::CameraKeys::AspectRatioKey << impl.GetAspectRatio();
		e << Key << keys::CameraKeys::ProjectionTypeKey << Value << impl.GetProjectionType().ToUnderlying();

		e << Key << keys::CameraKeys::PerspectiveKey << Value;
		e << BeginMap;
		e << Key << keys::CameraKeys::PerspectiveFOVKey << Value << impl.GetPerspectiveVerticalFOV();
		e << Key << keys::CameraKeys::PerspectiveNearKey << Value << impl.GetPerspectiveNearClip();
		e << Key << keys::CameraKeys::PerspectiveFarKey << Value << impl.GetPerspectiveFarClip();
		e << EndMap;

		e << Key << keys::CameraKeys::OrthographicKey << Value;
		e << BeginMap;
		e << Key << keys::CameraKeys::OrthographicZoomKey << Value << impl.GetOrthographicZoom();
		e << Key << keys::CameraKeys::OrthographicNearKey << Value << impl.GetOrthographicNearClip();
		e << Key << keys::CameraKeys::OrthographicFarKey << Value << impl.GetOrthographicFarClip();
		e << EndMap;

		e << EndMap;
		return e;
	}
}

Emitter& operator<<(Emitter& emitter, const std::string_view& str)
{
	return emitter.Write(str.data());
}

Emitter& operator<<(Emitter& emitter, const Entity& entity)
{
	const auto& tag = entity.GetComponent<TagComponent>();
	KRK_TRC(util::Fmt("Serializing entity: [{0},{1}]", entity.GetID(), tag.GetTag()));
	emitter << BeginMap;
	emitter << Key << "Entity" << Value << entity.GetID().GetValue();
	emitter << Key << "Components" << Value;
	emitter << BeginMap;
	if (entity.HasComponent<TagComponent>())
		emitter << Key << "Tag" << Value << tag;
	if (entity.HasComponent<TransformComponent>())
		emitter << Key << "Transform" << Value << entity.GetComponent<TransformComponent>();
	if (entity.HasComponent<Appearance2DComponent>())
		emitter << Key << "Appearance" << Value << entity.GetComponent<Appearance2DComponent>();
	if (entity.HasComponent<CameraComponent>())
		emitter << Key << "Camera" << Value << entity.GetComponent<CameraComponent>();
	// if (entity.HasComponent<NativeScriptComponent>())
		// emitter << Key << "Script" << Value << entity.GetComponent<NativeScriptComponent>().;
	emitter << EndMap;
	emitter << EndMap;

	return emitter;
}

namespace krakoa::scene::serialization::impl
{
	std::string SerializeScene(const iScene& scene)
	{
		const auto sceneName = std::string(scene.GetName());

		KRK_INF(util::Fmt("Serializing scene: {0}", sceneName));

		Emitter emitter;

		emitter << BeginMap;
		emitter << Key << keys::SceneKey << Value << sceneName;
		emitter << Key << keys::EntitiesKey << Value;
		emitter << BeginSeq;
		scene.ForEach([&emitter](const Entity& entity)
		{
			if (entity.GetID().IsNull())
				return;


			emitter << entity;

			if (!emitter.good())
			{
				KRK_ERR(util::Fmt("Failed to serialize entity: {0}\n"
					"Reason: {1}", entity.GetID(), emitter.GetLastError()));
			}
		});
		emitter << EndSeq;
		emitter << EndMap;

		if (!emitter.good())
		{
			KRK_ERR(util::Fmt("Failed to serialize scene: {0}", emitter.GetLastError()));
		}

		return emitter.c_str();
	}

	bool DeserializeScene(iScene& scene, const std::string& sceneData)
	{
		const auto rootNode = Load(sceneData);

		if (!rootNode[keys::RootNode])
			return false;

		const auto sceneName = rootNode[keys::SceneKey].as<std::string>();
		KRK_DBG("Deserializing scene: " + sceneName);

		const auto entitiesNode = rootNode[keys::EntitiesKey];

		if (!entitiesNode)
			return false;

		for (auto&& node : entitiesNode)
		{
			const ecs::EntityUID eID = node[keys::EntityKey].as<ecs::EntityUID::ID_t>();
			const auto componentNodes = node[keys::ComponentKeys::ComponentsKey];

			const auto tag = componentNodes[keys::ComponentKeys::TagKey].as < std::string >();
			KRK_DBG(util::Fmt("Deserializing entity: {0}", tag));

			auto& entity = scene.AddEntity(tag);

			if (const auto transformNode = componentNodes[keys::ComponentKeys::TransformKey]; transformNode)
			{
				const auto position = transformNode[keys::TransformKeys::PositionKey].as<maths::Vector3f>();
				const auto rotation = transformNode[keys::TransformKeys::RotationKey].as<maths::Vector3f>();
				const auto scale = transformNode[keys::TransformKeys::ScaleKey].as<maths::Vector3f>();

				if (!entity.HasComponent<TransformComponent>())
					entity.AddComponent<TransformComponent>(position, rotation, scale);
				else
				{
					auto& transform = entity.GetComponent<TransformComponent>();
					transform.SetPosition(position);
					transform.SetRotation(rotation);
					transform.SetScale(scale);
				}
			}

			if (const auto cameraNode = componentNodes[keys::ComponentKeys::CameraKey]; cameraNode)
			{
				SceneCamera::PerspectiveSpecs perspective;
				SceneCamera::OrthographicSpecs orthographic;
				
				const auto isPrimary = cameraNode[keys::CameraKeys::IsPrimaryKey].as<bool>();
				const auto aspectRatio = cameraNode[keys::CameraKeys::AspectRatioKey].as<float>();
				const SceneCamera::ProjectionType projectionType = cameraNode[keys::CameraKeys::ProjectionTypeKey].as<SceneCamera::ProjectionType::Underlying_t>();

				const auto perspectiveNode = cameraNode[keys::CameraKeys::PerspectiveKey];
				perspective.fov = perspectiveNode[keys::CameraKeys::PerspectiveFOVKey].as<float>();
				perspective.nearClip = perspectiveNode[keys::CameraKeys::PerspectiveNearKey].as<float>();
				perspective.farClip = perspectiveNode[keys::CameraKeys::PerspectiveFarKey].as<float>();

				const auto orthographicNode = cameraNode[keys::CameraKeys::OrthographicKey];
				orthographic.zoomScale = orthographicNode[keys::CameraKeys::OrthographicZoomKey].as<float>();
				orthographic.nearClip = orthographicNode[keys::CameraKeys::OrthographicNearKey].as<float>();
				orthographic.farClip = orthographicNode[keys::CameraKeys::OrthographicFarKey].as<float>();

				auto& camera = entity.AddComponent<CameraComponent>(new SceneCamera({}), isPrimary);
				camera.SetAspectRatio(aspectRatio);
				
				auto& sceneCam = camera.GetCamera<SceneCamera>();
				sceneCam.SetPerspective(perspective);
				sceneCam.SetOrthographic(orthographic);
				sceneCam.SetProjectionType(projectionType);
			}

			if (const auto appearanceNode = componentNodes[keys::ComponentKeys::AppearanceKey]; appearanceNode)
			{
				const gfx::GeometryType geometry = appearanceNode[keys::AppearanceKeys::GeometryTypeKey].as<gfx::GeometryType::Underlying_t>();
				const gfx::Colour colour = appearanceNode[keys::AppearanceKeys::ColourKey].as<gfx::Colour>();
				const auto tilingFactor = appearanceNode[keys::AppearanceKeys::TilingFactorKey].as<float>();

				// const auto textureID = appearanceNode[keys::AppearanceKeys::TextureIDKey].as<gfx::AssetID>();
				// const auto texCoord = appearanceNode[keys::AppearanceKeys::TexCoordKey].as<maths::Vector2f>();
				entity.AddComponent<Appearance2DComponent>(geometry, colour, tilingFactor);
			}

		}

		return true;
	}
}
