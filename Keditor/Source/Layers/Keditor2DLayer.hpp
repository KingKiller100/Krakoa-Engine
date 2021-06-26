#pragma once

#include <Layers/LayerBase.hpp>
#include <Graphics/2D/Textures/SubTexture2d.hpp>
#include <Camera/OrthographicCameraController.hpp>

#include <Graphics/Framebuffers/iFrameBuffer.hpp>

#include <Maths/Vectors/Vector4.hpp>

#include "../Panels/SceneHierarchyPanel.hpp"


#include <Scene/iScene.hpp>
#include <Scene/Serialization/SceneSerializer.hpp>

namespace krakoa
{
	namespace panels
	{
		class MenuBar;
	}
	
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
		void UpdateEntities() const noexcept;
		void UpdateViewport() noexcept;

		void SetUpScene();

		void ToggleScenePlayState() const;

		void InitializeMenuBar();
	
	private:
		Application& application;
		
		Solo_Ptr<gfx::SubTexture2D> pSubTexture;

		OrthographicCameraController cameraController;

		kmaths::Vector4f geometryColour;
		kmaths::Vector3f position;

		scene::serialization::SceneSerializer sceneSerializer;
		Solo_Ptr<panels::MenuBar> menuBar;
		
		bool isWindowFocused;
		bool isWindowHovered;

		scene::panels::SceneHierarchyPanel sceneHierarchyPanel;
	};
}
