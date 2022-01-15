#include "TagProperties.hpp"

#include "Scene/Entity/Components/TagComponent.hpp"
#include "../../../Logging/EditorLogger.hpp"

namespace krakoa::scene::panels
{
	using namespace ecs;

	TagProperties::TagProperties() noexcept
		= default;

	TagProperties::~TagProperties()
		= default;

	void TagProperties::DisplayProperties(Entity& entity)
	{
		KRK_PROFILE_FUNCTION();

		DrawComponent<components::TagComponent>("Tag", entity, [](components::TagComponent& tag)
		{
			constexpr auto flags = ui::InputTextFlags::EnterReturnsTrue | ui::InputTextFlags::CharsNoBlank;

			char buffer[1 << 8];
			std::strcpy(buffer, tag.GetRawTag());

			ui::DrawInputTextBox("Name", buffer, sizeof(buffer), flags,
				[&]()
			{
				if (klib::IsWhiteSpaceOrNull(buffer))
					return;

				LogEditorInfo(klib::ToString("Renaming entity: \"{0}\"->\"{1}\"", tag.GetTag(), buffer));

				tag.SetTag(buffer);
			});
		}
		, false);
	}
}
