#include "TransformProperties.hpp"

#include <Scene/Entity/Components/TransformComponent.hpp>

#include <Maths/Vectors/VectorMathsHelper.hpp>

namespace krakoa::scene::panels
{
	TransformProperties::TransformProperties() noexcept
		= default;

	TransformProperties::~TransformProperties()
		= default;

	void TransformProperties::DisplayProperties(ecs::Entity& entity)
	{
		using namespace ecs;
		
		KRK_PROFILE_FUNCTION();

		DrawComponent<components::TransformComponent>("Transform", entity, [](components::TransformComponent& transform)
		{
			constexpr auto btnColours = std::array{
				graphics::colours::Red,
				graphics::colours::Green,
				graphics::colours::Blue,
			};

			auto position = transform.GetPosition();
			auto scale = transform.GetScale();
			auto rotation = ToDegrees(transform.GetRotation());

			ui::DrawVec3Controller("Position", position, btnColours, 0.05f);
			ui::DrawVec3Controller("Rotation", rotation, btnColours, 0.5f);
			ui::DrawVec3Controller("Scale", scale, btnColours, 0.5f, 1.f);

			transform.SetPosition(position);
			transform.SetRotation(ToRadians(rotation));
			transform.SetScale(scale);
		}
		, false);
	}
}
