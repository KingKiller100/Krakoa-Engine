#pragma once

#include <Layers/LayerBase.hpp>
#include <Graphics/2D/Textures/SubTexture2d.hpp>
#include <Camera/OrthographicCameraController.hpp>

#include <Graphics/Framebuffers/iFrameBuffer.hpp>

#include <Maths/Vectors/Vector4.hpp>

class Keditor2DLayer : public krakoa::LayerBase
{
public:
	Keditor2DLayer() noexcept;

	void OnAttach() override;

	void OnDetach() override;

	void OnUpdate(float deltaTime) override;

	void OnRender() override;

	void OnEvent(krakoa::events::Event& e) override;

private:
	void SendRendererCommands() noexcept;
	void RenderZoomControls() noexcept;

	void SetUpEntities() const;

private:
	krakoa::Solo_Ptr<krakoa::graphics::SubTexture2D> pSubTexture;

	krakoa::OrthographicCameraController cameraController;
	
	kmaths::Vector4f geometryColour;
	kmaths::Vector3f position;
	float rotation = 0.f;
};