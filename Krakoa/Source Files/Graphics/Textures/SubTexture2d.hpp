#pragma once

#include "../../MemoryTypes.hpp"
#include <Maths/Vectors/Vector2.hpp>
#include <vector>

namespace krakoa::graphics
{
	class iTexture2D;

	using TexCoordsList = std::vector<kmaths::Vector2f>;

	struct TexCoordData
	{
		kmaths::Vector2f coordIndex;
		kmaths::Vector2f spriteDimensions;
		TexCoordsList baseCoords;
	};
	
	class SubTexture2D
	{
	public:
		// Only for quads
		explicit SubTexture2D(iTexture2D* texture, const TexCoordsList& texCoords);
		explicit SubTexture2D(const std::shared_ptr<iTexture2D>& texture, const TexCoordsList& texCoords);
		~SubTexture2D();
		
		USE_RESULT const Multi_Ptr<iTexture2D>& GetTexture() const noexcept;
		USE_RESULT Multi_Ptr<iTexture2D>& GetTexture() noexcept;
		USE_RESULT const kmaths::Vector2f* GetTexCoord() const noexcept;

		void SetTexture(iTexture2D* tex) noexcept;
		SETTER(std::shared_ptr<iTexture2D>, SetTexture, texture)

		// Only for quads
		static SubTexture2D* Create(iTexture2D* texture, const TexCoordData& data);
		static SubTexture2D* Create(const std::shared_ptr<iTexture2D > & texture, const TexCoordData& data);
		
	private:
		Multi_Ptr<iTexture2D> texture;
		std::vector<kmaths::Vector2f> texCoords;
	};
}