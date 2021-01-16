#pragma once

#include <Layers/LayerBase.hpp>
#include <Graphics/2D/Textures/SubTexture2d.hpp>
#include <Camera/OrthographicCameraController.hpp>

#include <Graphics/Framebuffers/iFrameBuffer.hpp>

#include <Maths/Vectors/Vector4.hpp>

#include "Graphics/Helpers/Colour.hpp"


#include "Scene/Scene.hpp"

namespace krakoa
{
	class Application;
	
	class Keditor2DLayer : public LayerBase
	{
	public:
		Keditor2DLayer() noexcept;

		void OnAttach() override;

		void OnDetach() override;

		void OnUpdate(float deltaTime) override;

		void OnRender() override;

		void OnEvent(events::Event& e) override;

	private:
		void UpdateColour() noexcept;
		void RenderZoomControls() noexcept;
		void RenderColourControls() const noexcept;

		void SetUpScene() const;

	private:
		Application& application;
		
		Solo_Ptr<graphics::SubTexture2D> pSubTexture;

		OrthographicCameraController cameraController;

		kmaths::Vector4f geometryColour;
		kmaths::Vector3f position;

		kmaths::Vector2<std::uint32_t> viewportSize;

		float degreesRotation = 0.f;

		bool isWindowFocused;
		bool isWindowHovered;
	};
}
