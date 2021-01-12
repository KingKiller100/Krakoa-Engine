#pragma once

#include "Entity/EntityComponentSystem.hpp"
#include <string>

namespace krakoa::scene
{
	class Scene
	{
	public:
		Scene();
		~Scene();

		void OnUpdate(float time);

	private:
		std::string name;
		EntityComponentSystem* ecs;
	};
}