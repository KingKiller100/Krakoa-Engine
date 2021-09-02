#pragma once

#include "OrthographicCamera.hpp"
#include "../EventsSystem/MouseEvent.hpp"
#include "../EventsSystem/ApplicationEvent.hpp"

#include <Maths/Vectors/Vector3.hpp>

namespace krakoa
{
	class OrthographicCameraController
	{
	public:
		OrthographicCameraController(float aspectRatio, bool isRotationAllowed = false);
		~OrthographicCameraController();

		void OnUpdate(const float deltaTime) noexcept;
		void OnEvent(events::Event& e);

		OrthographicCamera& GetCamera() noexcept;
		const OrthographicCamera& GetCamera() const noexcept;

		void SetRotationSpeed(const float rotSpeed) noexcept;
		void SetTranslationSpeed(const float transSpeed) noexcept;

		void Resize(float width, float height);
		void Resize(const maths::Size& dimensions);
		
		const iCamera::Bounds& GetBounds() const;

	private:
		bool OnMouseScrolledEvent(const events::MouseScrolledEvent& e);
		bool OnWindowResizeEvent(const events::WindowResizeEvent& e);

		void UpdateCameraProjectionMat() noexcept;

	private:
		float aspectRatio; // width / height
		float zoomLevel;
		bool isRotationAllowed;
		iCamera::Bounds bounds;
		OrthographicCamera camera;

		float rotationZ;
		kmaths::Vector3f position;

		float camRotationSpeed;
		float camTranslationSpeed;
	};
}