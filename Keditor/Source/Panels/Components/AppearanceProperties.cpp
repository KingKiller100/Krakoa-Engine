#include "AppearanceProperties.hpp"

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
			klib::type_trait::Traits<char>::Copy(buffer, geoType.ToString(), sizeof(buffer));

			ui::DrawInputTextBox("Geometry", buffer, sizeof(buffer), ui::InputTextFlags::ReadOnly | ui::InputTextFlags::NoMarkEdited);
			ui::DrawColourController("Colour", colour);

			appearance.SetColour(colour);
		});
	}
}
