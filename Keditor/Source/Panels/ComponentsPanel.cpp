#include "ComponentsPanel.hpp"

#include "Components/TagProperties.hpp"
#include "Components/CameraProperties.hpp"
#include "Components/TransformProperties.hpp"
#include "Components/AppearanceProperties.hpp"

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
#include <ImGui/imgui_internal.h>


namespace krakoa::scene::panels
{
	using namespace ecs;
	using namespace ui;

	namespace
	{
		template<typename Component>
		void DrawAddComponentMousePopupOption(const std::string_view& name, Entity& entity, const UICallBack& uiPopupFunc)
		{
			if (!entity.HasComponent<Component>())
			{
				popups::DrawPopupOption(name, uiPopupFunc);
			}
		}
	}

	ComponentsPanel::ComponentsPanel(const Multi_Ptr<EntityUID>& pSelected) noexcept
		: pSelectedEntity(pSelected)
	{
		properties.emplace_back(new TagProperties());
		properties.emplace_back(new TransformProperties());
		properties.emplace_back(new CameraProperties());
		properties.emplace_back(new AppearanceProperties());
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

		DrawPanel("Components", [&]()
			{
				if (pSelectedEntity.expired())
					return;

				const auto& id = *pSelectedEntity.lock();
				if (!id)
					return;

				DisplayComponents(id, scene);

				DrawAddComponentButton(scene, id);
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
	}

	void ComponentsPanel::DrawAddComponentButton(iScene& scene, const EntityUID& id)
	{
		const char btnName[] = "Add Component";

		DrawButton(btnName, {}, [&]() {
			popups::OpenPopup(btnName);
			});

		popups::DrawPopup(btnName, [&]()
			{
				auto& entity = scene.GetEntity(id);

				DrawAddComponentMousePopupOption<components::CameraComponent>("Camera", entity, [&]()
					{
						KRK_NRM(klib::ToString("Adding component \"Camera\" to entity {0}",
							entity.GetID())
						);

						const auto& window = GetApp().GetWindow();
						const auto size = Vector2f(window.GetDimensions());
						const auto aspectRatio = size.x / size.y;
						entity.AddComponent<components::CameraComponent>(
							new SceneCamera(iCamera::Bounds{ -aspectRatio, aspectRatio, -1.f, 1.f })
							);
					});

				DrawAddComponentMousePopupOption<components::Appearance2DComponent>("Appearance", entity, [&]()
					{
						KRK_NRM(klib::ToString("Adding component \"Appearance\" to entity {0}",
							entity.GetID())
						);

						entity.AddComponent<components::Appearance2DComponent>(
							graphics::GeometryType::QUAD,
							graphics::colours::White
							);
					});

				DrawAddComponentMousePopupOption<components::NativeScriptComponent>("Script", entity, [&]()
					{
						popups::DrawPopupOption("Script", [&]()
							{
								KRK_NRM(klib::ToString("Adding component \"Native Script\" to entity {0}",
									entity.GetID())
								);

								entity.AddComponent<components::NativeScriptComponent>();
							});
					});
			});
	}
}
