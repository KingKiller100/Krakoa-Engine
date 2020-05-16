#pragma once

#include "Colour.hpp"
#include "Textures/iTexture2D.hpp"
#include "Primitives 2D/BatchRendererData.hpp"

#include <Maths/Vectors/PredefinedVectors.hpp>


namespace krakoa { class OrthographicCamera; }

namespace krakoa::graphics
{
	class Renderer2D
	{
		// Statistics
		struct Statistics
		{
			size_t quadDrawCallsCount = 0;
			size_t triangleDrawCallsCount = 0;

			size_t quadCount = 0;
			size_t triangleCount = 0;

			void Reset() noexcept
			{
				quadDrawCallsCount = 0;
				triangleDrawCallsCount = 0;

				quadCount = 0;
				triangleCount = 0;
			}

			size_t TotalGeometry() const
			{
				return quadCount + triangleCount;
			}

			size_t TotalDrawCalls() const
			{
				return quadDrawCallsCount + triangleDrawCallsCount;
			}

			size_t TotalQuadVertexCount() const noexcept
			{
				return quadCount * batch::limits::quad::vertices;
			}

			size_t TotalQuadIndexCount() const noexcept
			{
				return quadCount * batch::limits::quad::indices;
			}

			size_t TotalTriangleVertexCount() const noexcept
			{
				return triangleCount * batch::limits::triangle::vertices;
			}

			size_t TotalTriangleIndexCount() const noexcept
			{
				return triangleCount * batch::limits::triangle::indices;
			}
		};

		inline static Statistics stats;

	public:
		static void Initialize();
		static void ShutDown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void FlushAll();
		static void FlushQuads();
		static void FlushTriangles();

		static void DrawTriangle(const Colour colour, const kmaths::Vector2f& position, const kmaths::Vector2f& scale = kmaths::Vector2f(1.f));
		static void DrawTriangle(const Colour colour, const kmaths::Vector3f& position, const kmaths::Vector2f& scale = kmaths::Vector2f(1.f));

		static void DrawQuad(const Colour colour, const kmaths::Vector2f& position, const kmaths::Vector2f& scale = kmaths::Vector2f(1.f));
		static void DrawQuad(const Colour colour, const kmaths::Vector3f& position, const kmaths::Vector2f& scale = kmaths::Vector2f(1.f));

		static void DrawTriangle(const std::shared_ptr<iTexture2D>& texture, const kmaths::Vector2f& position, const kmaths::Vector2f& scale = kmaths::Vector2f(1.f), const Colour tintColour = colours::White, const float tilingFactor = 1.f);
		static void DrawTriangle(const std::shared_ptr<iTexture2D>& texture, const kmaths::Vector3f& position, const kmaths::Vector2f& scale = kmaths::Vector2f(1.f), const Colour tintColour = colours::White, const float tilingFactor = 1.f);

		static void DrawQuad(const std::shared_ptr<iTexture2D>& texture, const kmaths::Vector2f& position, const kmaths::Vector2f& scale = kmaths::Vector2f(1.f), const Colour tintColour = colours::White, const float tilingFactor = 1.f);
		static void DrawQuad(const std::shared_ptr<iTexture2D>& texture, const kmaths::Vector3f& position, const kmaths::Vector2f& scale = kmaths::Vector2f(1.f), const Colour tintColour = colours::White, const float tilingFactor = 1.f);

		static void DrawRotatedTriangle(const Colour colour, const kmaths::Vector2f& position, const float degreesOfRotation = 0.f, const kmaths::Vector2f& scale = kmaths::Vector2f(1.f));
		static void DrawRotatedTriangle(const Colour colour, const kmaths::Vector3f& position, const float degreesOfRotation = 0.f, const kmaths::Vector2f& scale = kmaths::Vector2f(1.f));

		static void DrawRotatedTriangle(const std::shared_ptr<iTexture2D>& texture, const kmaths::Vector2f& position, const float degreesOfRotation = 0.f, const kmaths::Vector2f& scale = kmaths::Vector2f(1.f), const Colour tintColour = colours::White, const float tilingFactor = 1.f);
		static void DrawRotatedTriangle(const std::shared_ptr<iTexture2D>& texture, const kmaths::Vector3f& position, const float degreesOfRotation = 0.f, const kmaths::Vector2f& scale = kmaths::Vector2f(1.f), const Colour tintColour = colours::White, const float tilingFactor = 1.f);

		static void DrawRotatedQuad(const Colour colour, const kmaths::Vector2f& position, const float degreesOfRotation = 0.f, const kmaths::Vector2f& scale = kmaths::Vector2f(1.f));
		static void DrawRotatedQuad(const Colour colour, const kmaths::Vector3f& position, const float degreesOfRotation = 0.f, const kmaths::Vector2f& scale = kmaths::Vector2f(1.f));

		static void DrawRotatedQuad(const std::shared_ptr<iTexture2D>& texture, const kmaths::Vector2f& position, const float degreesOfRotation = 0.f, const kmaths::Vector2f& scale = kmaths::Vector2f(1.f), const Colour tintColour = kmaths::Vector4f(1.f), const float tilingFactor = 1.f);
		static void DrawRotatedQuad(const std::shared_ptr<iTexture2D>& texture, const kmaths::Vector3f& position, const float degreesOfRotation = 0.f, const kmaths::Vector2f& scale = kmaths::Vector2f(1.f), const Colour tintColour = kmaths::Vector4f(1.f), const float tilingFactor = 1.f);

		static const Statistics& GetStats();

	private:
		static void RestartBatch() noexcept;
		static void QueryLimitsMet() noexcept;
		static float UpdateTextureList(const std::shared_ptr<iTexture2D>& texture) noexcept;

		static void AddNewQuad(const kmaths::Vector3f& position, const kmaths::Vector2f& scale, const Colour colour = colours::White, const float texIdx = 0.f, const float degreesOfRotation = 0.f, const float tilingFactor = 1.f);
		static void AddNewTriangle(const kmaths::Vector3f& position, const kmaths::Vector2f& scale, const Colour colour = colours::White, const float texIdx = 0.f, const float degreesOfRotation = 0.f, const float tilingFactor = 1.f);
	};
}