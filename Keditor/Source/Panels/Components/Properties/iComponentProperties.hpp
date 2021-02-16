#pragma once

#include <UI/ImGui/UI.hpp>
#include <Util/TypeInfo.hpp>
#include <Debug/Instrumentor.hpp>
#include <Scene/Entity/Entity.hpp>
#include <Logging/EngineLogger.hpp>

#include <ImGui/imgui.h>
#include <ImGui/imgui_internal.h>

#include <string_view>

namespace krakoa {
	namespace scene {
		namespace panels
		{
			class iComponentProperties
			{
			public:
				virtual ~iComponentProperties() = default;

				virtual void DisplayProperties(ecs::Entity& entity) = 0;
			};


			template<typename Component, typename UILayoutFunc>
			void DrawComponent(const std::string_view& name, ecs::Entity& entity
				, const UILayoutFunc& uiLayoutFunc, bool removable = true)
			{
				using namespace ui;

				KRK_PROFILE_FUNCTION();

				if (!entity.HasComponent<Component>())
					return;

				const auto contentRegionAvail = GetContentRegionAvailable();

				constexpr auto treeNodeFlags = TreeNodeFlags::DefaultOpen | TreeNodeFlags::Framed
					| TreeNodeFlags::SpanAvailWidth | TreeNodeFlags::AllowItemOverlap;

				auto& component = entity.GetComponent<Component>();

				if (removable)
				{
					PushStyleVar(StyleVarFlags::FramePadding, { 4.f, 4.f });
					DrawSeparator();
				}

				ui::DrawTreeNode(name, (void*)util::GetTypeHash<Component>(), treeNodeFlags, [&](bool opened)
				{
					bool markedComponentForRemoval = false;

					if (removable)
					{
						const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
						DrawSameLine(contentRegionAvail.x - lineHeight * .5f);

						const char popupMenuId[] = "Settings";
						ui::DrawButton("+", { lineHeight, lineHeight }, [&]
						{
							popups::OpenPopup(popupMenuId);
						});
						PopStyleVar();

						popups::DrawPopup(popupMenuId, [&] {
							ui::DrawMenuItem("Remove", [&]
							{
								KRK_NRM(klib::ToString("Removing component \"{0}\" from entity {1}",
									util::GetTypeNameNoNamespace<Component>()
									, entity.GetID()));
								markedComponentForRemoval = true;
							});
						});
					}

					if (opened)
					{
						uiLayoutFunc(component);
					}

					if (markedComponentForRemoval)
						entity.RemoveComponent<Component>();
				});
			}
		}
	}
}

