#pragma once

#include "../Primitives2D/BatchRendererData.hpp"
#include "../../../Core/PointerTypes.hpp"
#include <Maths/Vectors/Vector2.hpp>
#include <Utility/Enum/kEnum.hpp>
#include <vector>
#include <cstdint>

namespace krakoa::graphics
{
	class iTexture2D;

	using TexCoordsList = std::vector<kmaths::Vector2f>;

	ENUM_CLASS(GeometryType, std::uint8_t,
		QUAD = batch::limits::quad::vertices,
		CIRCLE = batch::limits::circle::vertices,
		TRIANGLE = batch::limits::triangle::vertices,
		UNKNOWN = 255
	);

	class SubTexture2D
	{
	public:
		struct TexCoordData
		{
			kmaths::Vector2f coordIndex;
			kmaths::Vector2f spriteDimensions;
			TexCoordsList baseCoords;
		};

	public:
		SubTexture2D(GeometryType geo);
		SubTexture2D(iTexture2D* texture, const TexCoordData& data);
		SubTexture2D(const std::shared_ptr<iTexture2D>& texture, const TexCoordData& data);
		~SubTexture2D() noexcept;

		USE_RESULT const Multi_Ptr<iTexture2D>& GetTexture() const noexcept;
		USE_RESULT Multi_Ptr<iTexture2D>& GetTexture() noexcept;
		USE_RESULT const kmaths::Vector2f* GetTexCoord() const noexcept;

		void SetTexture(iTexture2D* tex) noexcept;
		void SetTexture(const std::shared_ptr<iTexture2D>& value) noexcept { (texture) = value; }

		USE_RESULT const TexCoordData& GetTexCoordData() const noexcept { return texCoordData; }

		USE_RESULT GeometryType GetGeometryType() const noexcept;
		
		// Only for quads
		static SubTexture2D* Create(iTexture2D* texture, const TexCoordData& texCoordData);
		static SubTexture2D* Create(const std::shared_ptr<iTexture2D > & texture, const TexCoordData& data);

	private:
		void CreateTexCoords();
		GeometryType DeduceGeometryType() const;
		
	private:
		Multi_Ptr<iTexture2D> texture;
		std::vector<kmaths::Vector2f> texCoords;
		TexCoordData texCoordData;
		GeometryType geometry;
	};
}
