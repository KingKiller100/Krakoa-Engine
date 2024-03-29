﻿#pragma once
#include "Camera.hpp"

#include <Utility/Enum/kEnum.hpp>
#include <Maths/Vectors/Vector2.hpp>

namespace krakoa
{
	class SceneCamera : public Camera
	{
	public:
		ENUM_CLASS(ProjectionType, int,
			Orthographic
			, Perspective
		);

		struct OrthographicSpecs
		{
			float zoomScale = 10.f;
			float nearClip = -1.f;
			float farClip = 1.0f;
		};
		
		struct PerspectiveSpecs
		{
			float fov = kmaths::ToRadians(45.0f);
			float nearClip = 0.01f;
			float farClip = 1000.0f;
		};

	public:
		SceneCamera(const Bounds& bounds, float nearClip = -1.f, float farClip = 1.f);
		~SceneCamera() noexcept override;

		void SetPerspective(const PerspectiveSpecs& specs);
		void SetPerspective(float verticalFOV, float nearClip, float farClip);

		void SetOrthographic(const OrthographicSpecs& specs);
		void SetOrthographic(float zoom, float nearClip, float farClip);

		void SetViewportSize(std::uint32_t width, std::uint32_t height);
		void SetViewportSize(const maths::BasicSize<std::uint32_t>& dimensions);


		float GetPerspectiveVerticalFOV() const;
		void SetPerspectiveVerticalFOV(float verticalFov);
		float GetPerspectiveNearClip() const;
		void SetPerspectiveNearClip(float nearClip);
		float GetPerspectiveFarClip() const;
		void SetPerspectiveFarClip(float farClip);

		float GetOrthographicZoom() const;
		void SetOrthographicZoom(float size);
		float GetOrthographicNearClip() const;
		void SetOrthographicNearClip(float nearClip);
		float GetOrthographicFarClip() const;
		void SetOrthographicFarClip(float farClip);


		ProjectionType GetProjectionType() const;
		void SetProjectionType(ProjectionType projType);

	private:
		void UpdateProjection();
		
	private:
		ProjectionType projectionType;
		OrthographicSpecs orthoSpecs;
		PerspectiveSpecs perspSpecs;
	};
}
