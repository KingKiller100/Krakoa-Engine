#pragma once

#include "Textures/iTexture2D.hpp"

#include <Maths/Vectors/PredefinedVectors.hpp>

namespace krakoa { class OrthographicCamera; }

namespace krakoa::graphics
{
	class Renderer2D
	{
	public:
		static void Initialize();
		static void ShutDown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		static void Flush();

		static void DrawTriangle(const kmaths::Vector4f& colour, const kmaths::Vector2f& position, const kmaths::Vector2f& scale = kmaths::Vector2f(1.f));
		static void DrawTriangle(const kmaths::Vector4f& colour, const kmaths::Vector3f& position, const kmaths::Vector2f& scale = kmaths::Vector2f(1.f));

		static void DrawQuad(const kmaths::Vector4f& colour, const kmaths::Vector2f& position, const kmaths::Vector2f& scale = kmaths::Vector2f(1.f));
		static void DrawQuad(const kmaths::Vector4f& colour, const kmaths::Vector3f& position, const kmaths::Vector2f& scale = kmaths::Vector2f(1.f));

		static void DrawQuad(const std::shared_ptr<iTexture2D>& texture, const kmaths::Vector2f& position, const kmaths::Vector2f& scale = kmaths::Vector2f(1.f), const kmaths::Vector4f& tintColour = kmaths::Vector4f(1.f), const float tilingFactor = 1.f);
		static void DrawQuad(const std::shared_ptr<iTexture2D>& texture, const kmaths::Vector3f& position, const kmaths::Vector2f& scale = kmaths::Vector2f(1.f), const kmaths::Vector4f& tintColour = kmaths::Vector4f(1.f), const float tilingFactor = 1.f);

		static void DrawRotatedTriangle(const kmaths::Vector4f& colour, const kmaths::Vector2f& position, const float degreesOfRotation = 0.f, const kmaths::Vector2f& scale = kmaths::Vector2f(1.f));
		static void DrawRotatedTriangle(const kmaths::Vector4f& colour, const kmaths::Vector3f& position, const float degreesOfRotation = 0.f, const kmaths::Vector2f& scale = kmaths::Vector2f(1.f));

		static void DrawRotatedQuad(const kmaths::Vector4f& colour, const kmaths::Vector2f& position, const float degreesOfRotation = 0.f, const kmaths::Vector2f& scale = kmaths::Vector2f(1.f));
		static void DrawRotatedQuad(const kmaths::Vector4f& colour, const kmaths::Vector3f& position, const float degreesOfRotation = 0.f, const kmaths::Vector2f& scale = kmaths::Vector2f(1.f));

		static void DrawRotatedQuad(const std::shared_ptr<iTexture2D>& texture, const kmaths::Vector2f& position, const float degreesOfRotation = 0.f, const kmaths::Vector2f& scale = kmaths::Vector2f(1.f), const kmaths::Vector4f& tintColour = kmaths::Vector4f(1.f), const float tilingFactor = 1.f);
		static void DrawRotatedQuad(const std::shared_ptr<iTexture2D>& texture, const kmaths::Vector3f& position, const float degreesOfRotation = 0.f, const kmaths::Vector2f& scale = kmaths::Vector2f(1.f), const kmaths::Vector4f& tintColour = kmaths::Vector4f(1.f), const float tilingFactor = 1.f);

	private:
		static void Restart() noexcept;
		static void QueryLimitsMet() noexcept;
		static void AddNewQuad(const kmaths::Vector3f& position, const kmaths::Vector2f& scale, const kmaths::Vector4f& colour = kmaths::Vector4f(1.f), const float texIdx = 0.f, const float degreesOfRotation = 0.f, const float tilingFactor = 1.f);
	};
}