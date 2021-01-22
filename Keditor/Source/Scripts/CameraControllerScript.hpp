#pragma once

#include <Krakoa.hpp>

#include <Maths/Vectors/Vector3.hpp>

namespace krakoa
{
	namespace scene::ecs::components
	{
		class TransformComponent;
	}

	class CameraControllerScript : public scene::ecs::ScriptEntity
	{
	public:
		void OnCreate() override
		{
			transform = GetComponent<scene::ecs::components::TransformComponent>();
		}

		void OnUpdate(float deltaTime) override
		{
			constexpr auto up = Vector3f(0, 1, 0);
			constexpr auto down = Vector3f(0, -1, 0);
			constexpr auto left = Vector3f(-1, 0, 0);
			constexpr auto right = Vector3f(1, 0, 0);

			// KRK_DBG(klib::ToString<>("Delta Time: {0:3}", deltaTime));

			if (IsKeyPressed(input::KEY_W))
			{
				const auto& position = transform->GetPosition();
				const auto movement = up * deltaTime;
				transform->SetPosition(position + movement);
			}

			if (IsKeyPressed(input::KEY_S))
			{
				const auto& position = transform->GetPosition();
				const auto movement = down * deltaTime;
				transform->SetPosition(position + movement);
			}
		}

	private:
		scene::ecs::components::TransformComponent* transform = nullptr;
	};
}
