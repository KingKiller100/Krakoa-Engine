#pragma once

#include "iComponentProperties.hpp"

namespace krakoa::scene::panels
{
	class TagProperties final : public iComponentProperties
	{
	public:
		TagProperties() noexcept;
		~TagProperties() override;
		void DisplayProperties(ecs::Entity& entity) override;
	};
}
