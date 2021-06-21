#pragma once

namespace krakoa::scene::serialization::impl::keys
{
	static constexpr auto SceneKey = "Scene";
	static constexpr auto EntitiesKey = "Entities";
	static constexpr auto EntityKey = "Entity";
	static constexpr auto RootNode = SceneKey;
	
	struct ComponentKeys
	{
		static constexpr auto ComponentsKey = "Components";
		static constexpr auto AppearanceKey = "Appearance";
		static constexpr auto TransformKey = "Transform";
		static constexpr auto CameraKey = "Camera";
		static constexpr auto TagKey = "Tag";
		static constexpr auto NativeScriptKey = "Script";
	};

	struct TransformKeys
	{
		static constexpr auto PositionKey = "Position";
		static constexpr auto RotationKey = "Rotation";
		static constexpr auto ScaleKey = "Scale";
	};

	struct CameraKeys
	{
		static constexpr auto IsPrimaryKey = "Primary";
		static constexpr auto AspectRatioKey = "AspectRatio";
		static constexpr auto ProjectionTypeKey = "ProjectionTypeIndex";

		static constexpr auto PerspectiveKey = "Perspective";
		static constexpr auto PerspectiveFOVKey = "FieldOfView";
		static constexpr auto PerspectiveVFOVKey = "VerticalFieldOfView";
		static constexpr auto PerspectiveHFOVKey = "HorizontalFieldOfView";
		static constexpr auto PerspectiveNearKey = "Near";
		static constexpr auto PerspectiveFarKey = "Far";
		
		static constexpr auto OrthographicKey = "Orthographic";
		static constexpr auto OrthographicZoomKey = "Zoom";
		static constexpr auto OrthographicNearKey = "Near";
		static constexpr auto OrthographicFarKey = "Far";
	};

	struct AppearanceKeys
	{
		static constexpr auto GeometryTypeKey = "GeometryTypeIndex";
		static constexpr auto ColourKey = "Colour";
		static constexpr auto TilingFactorKey = "TilingFactor";
		static constexpr auto TextureIDKey = "TextureID";
		static constexpr auto TexCoordKey = "TexCoord";
		static constexpr auto TexCoordDataKey = "TexCoordData";
		static constexpr auto CoordIndexKey = "CoordIndex";
		static constexpr auto BaseCoordsKey = "BaseCoords";
		static constexpr auto SpriteDimensionsKey = "SpriteDimensions";
		
	};
}
