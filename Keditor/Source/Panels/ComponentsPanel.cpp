﻿#include "ComponentsPanel.hpp"

#include "Components/Properties/TagProperties.hpp"
#include "Components/Properties/CameraProperties.hpp"
#include "Components/Properties/TransformProperties.hpp"
#include "Components/Properties/AppearanceProperties.hpp"

#include "Components/Popup/AddScriptPopupOption.hpp"
#include "Components/Popup/AddCameraPopupOption.hpp"
#include "Components/Popup/AddAppearancePopupOption.hpp"

#include <Camera/SceneCamera.hpp>
#include <Core/Application.hpp>

#include <Scene/iScene.hpp>
#include <Scene/Entity/Entity.hpp>
#include <Scene/Entity/Components/TagComponent.hpp>
#include <Scene/Entity/Components/CameraComponent.hpp>
#include <Scene/Entity/Components/TransformComponent.hpp>
#include <Scene/Entity/Components/AppearanceComponent.hpp>
#include <Scene/Entity/Components/NativeScriptComponent.hpp>

#include <UI/ImGui/UI.hpp>

#include <Util/TypeInfo.hpp>

#include <ImGui/imgui.h>


namespace krakoa::scene::panels
{
	using namespace ecs;
	using namespace ui;

	ComponentsPanel::ComponentsPanel(const Multi_Ptr<EntityUID>& pSelected) noexcept
		: pSelectedEntity(pSelected)
	{
		InitializeProperties();
		InitializeAddCompPopupOptions();
	}

	void ComponentsPanel::InitializeProperties()
	{
		properties.emplace_back(new TagProperties());
		properties.emplace_back(new TransformProperties());
		properties.emplace_back(new CameraProperties());
		properties.emplace_back(new AppearanceProperties());
	}

	void ComponentsPanel::InitializeAddCompPopupOptions()
	{
		addCompPopupOptions.emplace_back(new AddCameraPopupOption);
		addCompPopupOptions.emplace_back(new AddAppearancePopupOption);
		addCompPopupOptions.emplace_back(new AddScriptPopupOption);
	}

	ComponentsPanel::~ComponentsPanel() noexcept
		= default;

	void ComponentsPanel::LogConnected() const
	{
		KRK_LOG("Keditor", "Components panel connected");
	}

	void ComponentsPanel::DrawActiveScene(iScene& scene)
	{
		KRK_PROFILE_FUNCTION();

		const auto winSize = GetWindow().GetDimensions();

		const auto minDim = winSize / kmaths::constants::OneOver<float>(6);
		const auto maxDim = minDim * 3;
		
		ui::SetNextWindowConstraints(minDim, maxDim);
		DrawPanel("Components", [&]()
		{
			if (pSelectedEntity.expired())
				return;

			const auto& id = *pSelectedEntity.lock();
			if (!id)
				return;

			DisplayComponents(id, scene);
		});
	}

	void ComponentsPanel::DrawNoActiveScene()
	{
		DrawPanel("Components", nullptr);
	}

	void ComponentsPanel::DisplayComponents(const EntityUID& id, iScene& scene)
	{
		if (!scene.HasEntity(id))
			return;

		// This is the draw order of the components
		auto& entity = scene.GetEntity(id);

		for (auto& property : properties)
		{
			property->DisplayProperties(entity);
		}

		DrawAddComponentButton(scene, entity);
	}

	void ComponentsPanel::DrawAddComponentButton(iScene& scene, Entity& entity)
	{
		//ImGui::PushItemWidth(-1);
		
		const char btnName[] = "Add Component";

		DrawButton(btnName, {}, [&]()
		{
			popups::OpenPopup(btnName);
		});

		popups::DrawPopup(btnName, [&]()
		{
			for (auto& option : addCompPopupOptions)
			{
				option->DisplayOption(entity);
			}
		});

		// ImGui::PopItemWidth();
	}
}
