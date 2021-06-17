#pragma once

#include "../Colour.hpp"
#include "Renderer2DStatistics.hpp"

#include "../../Scene/Entity/Components/AppearanceComponent.hpp"
#include "../../Scene/Entity/Components/TransformComponent.hpp"

#include "../2D/Textures/iTexture2D.hpp"

#include <Maths/Vectors/Vector2.hpp>
#include <Maths/Vectors/Vector3.hpp>

namespace krakoa
{
	class iCamera;
	class OrthographicCamera;

	namespace gfx
	{
		class ShaderLibrary;
		class iTexture2D;
		class SubTexture2D;
		
		class Renderer2D
		{
		public:
			static void Initialize(ShaderLibrary& shaderLibrary);
			static void ShutDown();

			static void BeginScene(const iCamera& camera, const kmaths::TransformMatrix<float>& transformMat);
			static void BeginScene(const OrthographicCamera& camera);
			static void EndScene();

			static void FlushAll();
			static void FlushQuads();
			static void FlushTriangles();

			static void DrawTriangle(const scene::ecs::components::Appearance2DComponent& appearance, const scene::ecs::components::TransformComponent& transform);
			static void DrawTriangle(const SubTexture2D& subTexture, const kmaths::Vector2f& position, const kmaths::Vector2f& scale = kmaths::Vector2f(1.f)
				, const float radians = 0.f, const Colour tintColour = colours::White, const float tilingFactor = 1.f);
			static void DrawTriangle(const SubTexture2D& subTexture, const kmaths::Vector3f& position, const kmaths::Vector2f& scale = kmaths::Vector2f(1.f)
				, const float radians = 0.f, const Colour tintColour = colours::White, const float tilingFactor = 1.f);

			static void DrawQuad(const scene::ecs::components::Appearance2DComponent& appearance, const scene::ecs::components::TransformComponent& transform);
			static void DrawQuad(const SubTexture2D& subTexture, const kmaths::Vector2f& position, const kmaths::Vector2f& scale = kmaths::Vector2f(1.f)
				, const float radians = 0.f, const Colour tintColour = colours::White, const float tilingFactor = 1.f);
			static void DrawQuad(const SubTexture2D& subTexture, const kmaths::Vector3f& position, const kmaths::Vector2f& scale = kmaths::Vector2f(1.f)
				, const float radians = 0.f, const Colour tintColour = colours::White, const float tilingFactor = 1.f);

			static const Statistics& GetStats();

		private:
			static void RestartBatch() noexcept;
			static void FlushIfLimitsMet() noexcept;
			static float UpdateTextureList(const std::shared_ptr<iTexture2D>& texture) noexcept;

			static void AddNewTriangle(const kmaths::Vector3f& position, const kmaths::Vector2f& scale, const float radians, const Colour colour, const float texIndex
				, const kmaths::Vector2f*& texCoords, const float tilingFactor);
			
			static void AddNewQuad(const kmaths::Vector3f& position, const kmaths::Vector2f& scale, const float radians, const Colour colour, float texIndex
				, const kmaths::Vector2f*& texCoords, const float tilingFactor);

			inline static Statistics stats;
		};
	}
}
