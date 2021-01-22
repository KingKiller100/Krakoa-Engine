#pragma once

#include "ColourChangeScript.hpp"

#include <Krakoa.hpp>

#include <Maths/Vectors/Vector3.hpp>

namespace krakoa
{
	class AnimateEntityScript final : public ColourChangeScript
	{
	public:
		void SetMoveSpeed(float speed)
		{
			moveSpeed = speed;
		}

		void SetRotationSpeed(float speed)
		{
			rotSpeed = speed;
		}
		
	protected:
		void OnCreate() override
		{
			transform = GetComponent<scene::ecs::components::TransformComponent>();
		}

		void OnUpdate(float deltaTime) override
		{
			UpdatePosition(deltaTime);
			UpdateRotation(deltaTime);
		}

	private:
		void UpdatePosition(float deltaTime)
		{
			constexpr auto up = Vector3f(0, 1, 0);
			constexpr auto down = Vector3f(0, -1, 0);
			constexpr auto left = Vector3f(-1, 0, 0);
			constexpr auto right = Vector3f(1, 0, 0);

			const auto& position = transform->GetPosition();

			const auto speed = moveSpeed * deltaTime;
			kmaths::Vector3f movement;
			
			if (input::InputManager::IsKeyPressed(input::KEY_RIGHT))
				movement += right * speed;

			if (input::InputManager::IsKeyPressed(input::KEY_LEFT))
				movement += left * speed;

			if (input::InputManager::IsKeyPressed(input::KEY_UP))
				movement += up * speed;

			if (input::InputManager::IsKeyPressed(input::KEY_DOWN))
				movement += down * speed;

			transform->SetPosition(position + movement);
		}

		void UpdateRotation(float deltaTime)
		{
			degreesRotation = rotSpeed * deltaTime;
			transform->SetRotation(kmaths::ToRadians(degreesRotation));
		}
		
	private:
		scene::ecs::components::TransformComponent* transform = nullptr;
		float moveSpeed = 0;
		float rotSpeed = 0;
		float degreesRotation = 0;
	};
}
