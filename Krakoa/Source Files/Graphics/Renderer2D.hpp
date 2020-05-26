﻿#pragma once

#include "Colour.hpp"
#include "Renderer2DStatistics.hpp"

#include <Maths/Vectors/PredefinedVectors.hpp>

namespace krakoa
{
	class OrthographicCamera;

	namespace graphics
	{
		class iTexture2D;
		class SubTexture2D;
		
		class Renderer2D
		{
		public:
			static void Initialize();
			static void ShutDown();

			static void BeginScene(const OrthographicCamera& camera);
			static void EndScene();

			static void FlushAll();
			static void FlushQuads();
			static void FlushTriangles();

			static void DrawTriangle(const std::shared_ptr<iTexture2D>& texture, const kmaths::Vector2f& position, const kmaths::Vector2f& scale = kmaths::Vector2f(1.f), const float radians = 0.f, const Colour tintColour = colours::White, const float tilingFactor = 1.f);
			static void DrawTriangle(const std::shared_ptr<iTexture2D>& texture, const kmaths::Vector3f& position, const kmaths::Vector2f& scale = kmaths::Vector2f(1.f), const float radians = 0.f, const Colour tintColour = colours::White, const float tilingFactor = 1.f);

			static void DrawQuad(const std::unique_ptr<SubTexture2D>& subTexture, const kmaths::Vector2f& position, const kmaths::Vector2f& scale = kmaths::Vector2f(1.f), const float radians = 0.f, const Colour tintColour = colours::White, const float tilingFactor = 1.f);
			static void DrawQuad(const std::unique_ptr<SubTexture2D>& subTexture, const kmaths::Vector3f& position, const kmaths::Vector2f& scale = kmaths::Vector2f(1.f), const float radians = 0.f, const Colour tintColour = colours::White, const float tilingFactor = 1.f);

			static const Statistics& GetStats();

		private:
			static void RestartBatch() noexcept;
			static void QueryLimitsMet() noexcept;
			static float UpdateTextureList(const std::shared_ptr<iTexture2D>& texture) noexcept;

			static void AddNewTriangle(const kmaths::Vector3f& position, const kmaths::Vector2f& scale, const float radians, const Colour colour, const float texIdx, const float tilingFactor);
			static void AddNewQuad(const kmaths::Vector3f& position, const kmaths::Vector2f& scale, const float radians, const Colour colour, float texIndex, const kmaths::Vector2f*& texCoords, const float tilingFactor);

			inline static Statistics stats;
		};
	}
}