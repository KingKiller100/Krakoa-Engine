#pragma once

#include "iComponentProperties.hpp"

namespace krakoa::scene::panels
{
	class TransformProperties final : public iComponentProperties
	{
	public:
		TransformProperties() noexcept;
		~TransformProperties() override;
		void DisplayProperties(ecs::Entity& entity) override;
	};
}
