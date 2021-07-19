#include "AppearanceProperties.hpp"



#include "Core/Application.hpp"
#include "FileSystem/AssetManager.hpp"
#include "Scene/Entity/Components/AppearanceComponent.hpp"

namespace krakoa::scene::panels
{
	AppearanceProperties::AppearanceProperties() noexcept
		= default;

	AppearanceProperties::~AppearanceProperties()
		= default;

	void AppearanceProperties::DisplayProperties(ecs::Entity& entity)
	{
		using namespace ecs;
		
		KRK_PROFILE_FUNCTION();

		DrawComponent<components::Appearance2DComponent>("Appearance", entity, [](components::Appearance2DComponent& appearance)
		{
			auto colour = appearance.GetColour();
			const auto geoType = appearance.GetGeometryType();
			// auto& subTexture = appearance.GetSubTexture().GetTexture()->;

			char buffer[1 << 8];
			klib::type_trait::Traits<char>::Copy(buffer, geoType.C_Str(), sizeof(buffer));

			const auto& assetManager = GetApp().GetManager<filesystem::AssetManager>();
			const auto& fontLib = assetManager.GetFontLibrary();
			const auto& font = fontLib.GetFont(gfx::Font::Modifiers::Bold);
			
			ui::DrawInputTextBox("Geometry", buffer, sizeof(buffer), ui::InputTextFlags::ReadOnly | ui::InputTextFlags::NoMarkEdited);
			ui::DrawColourController("Colour", colour, *font);

			appearance.SetColour(colour);
		});
	}
}
