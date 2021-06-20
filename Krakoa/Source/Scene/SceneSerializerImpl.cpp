#include "Precompile.hpp"
#include "SceneSerializerImpl.hpp"

#include "Entity/Entity.hpp"
#include "Entity/Components/TagComponent.hpp"
#include "Entity/Components/CameraComponent.hpp"
#include "Entity/Components/TransformComponent.hpp"
#include "Entity/Components/AppearanceComponent.hpp"
#include "Entity/Components/NativeScriptComponent.hpp"

#include "../Graphics/Colour.hpp"
#include "../Maths/Maths.hpp"
#include "../Camera/SceneCamera.hpp"

using namespace krakoa;
using namespace scene::ecs;
namespace YAML
{
	namespace
	{
		using namespace components;

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
			e << Key << "Position" << Value << pos;
			e << Key << "Rotation" << Value << rotation;
			e << Key << "Scale" << Value << scale;
			e << EndMap;
			return e;
		}

		Emitter& operator<<(Emitter& e, const Appearance2DComponent& appearance)
		{
			const auto subTexture = appearance.GetSubTexture();
			e << BeginMap;
			e << Key << "GeometryType" << Value << appearance.GetGeometryType().ToUnderlying();
			e << Key << "Colour" << Value << appearance.GetColour();
			e << Key << "TillingFactor" << Value << appearance.GetTilingFactor();
			e << Key << "TextureID" << Value;

			if (!subTexture.GetTexture())
				e << "N/A";
			else
			{
				e << subTexture.GetTexture()->GetAssetID().GetID();
				e << Key << "TexCoord" << Value << *subTexture.GetTexCoord();
				e << Key << "TexCoordData";
				e << BeginMap;
				e << Key << "BaseCoords" << Value;
				e << BeginSeq;
				for (auto&& coord : subTexture.GetTexCoordData().baseCoords)
				{
					e << coord;
				}
				e << EndSeq;
				e << Key << "CoordIndex" << Value << subTexture.GetTexCoordData().coordIndex;
				e << Key << "SpriteDimensions" << Value << subTexture.GetTexCoordData().spriteDimensions;
				e << EndMap;
			}
			e << EndMap;
			return e;
		}

		Emitter& operator<<(Emitter& e, const CameraComponent& camera)
		{
			const auto& impl = *camera.GetCamera<krakoa::SceneCamera>();

			e << BeginMap;
			e << Key << "Primary" << camera.IsPrimary();
			e << Key << "FixedAspectRatio" << impl.GetAspectRatio();
			e << Key << "ProjectionType" << Value << impl.GetProjectionType().ToUnderlying();
			e << Key << "PerspectiveFOV" << Value << impl.GetPerspectiveVerticalFOV();
			e << Key << "PerspectiveNear" << Value << impl.GetPerspectiveNearClip();
			e << Key << "PerspectiveFar" << Value << impl.GetPerspectiveFarClip();
			e << Key << "OrthographicZoom" << Value << impl.GetOrthographicZoom();
			e << Key << "OrthographicNear" << Value << impl.GetOrthographicNearClip();
			e << Key << "OrthographicFar" << Value << impl.GetOrthographicFarClip();
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
		emitter << BeginMap;
		if (entity.HasComponent<TagComponent>())
			emitter << Key << "Tag" << Value << entity.GetComponent<TagComponent>();
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
}
