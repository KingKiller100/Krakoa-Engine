#pragma once

#include <Layers/LayerBase.hpp>
#include <Rendering/Rendering Resources/iVertexArray.hpp>
#include <Rendering/Textures/iTexture2D.hpp>
#include <Camera/OrthographicCameraController.hpp>

#include <Maths/Vectors/Vector4.hpp>

class Renderer2DLayer : public krakoa::LayerBase
{
public:
	Renderer2DLayer() noexcept;

	void OnAttach() override;

	void OnDetach() override;

	void OnUpdate(float deltaTime) override;

	void OnRender() override;

	void OnEvent(krakoa::events::Event& e) override;

private:
	void SendRendererCommands() noexcept;
	void ZoomControlsDisplay() noexcept;

private:
	std::shared_ptr<krakoa::graphics::iTexture2D> pWinTexture;

	krakoa::OrthographicCameraController cameraController;

	kmaths::Vector4f geometryColour;
	kmaths::Vector3f position;
	float rotation = 0.f;
};