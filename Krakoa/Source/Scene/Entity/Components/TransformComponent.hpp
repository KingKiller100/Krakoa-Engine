#pragma once

#include "../../../Maths/Maths.hpp"

namespace krakoa::scene::ecs::components
{
	class TransformComponent
	{
	public:
		TransformComponent() noexcept;

		TransformComponent(const maths::Vector3f& position,
		                   const maths::Vector3f& rotation,
		                   const maths::Vector3f& scale = {1.f, 1.f, 1.f}) noexcept;

		~TransformComponent() noexcept;

		USE_RESULT maths::Vector3f GetPosition() const noexcept;
		void SetPosition(const maths::Vector3f& value) noexcept;

		USE_RESULT maths::Vector3f GetScale() const noexcept;
		void SetScale(const maths::Vector3f& value) noexcept;
		void SetScale(const maths::Vector2f& value);

		USE_RESULT maths::Vector3f GetRotation(maths::Theta theta) const noexcept;
		USE_RESULT maths::Vector3f GetRotation() const noexcept;
		void SetRotation(const maths::Vector3f& value) noexcept;
		void SetRotationX(const maths::Vector3f::Type& value) noexcept;
		void SetRotationY(const maths::Vector3f::Type& value) noexcept;
		void SetRotationZ(const maths::Vector3f::Type& value) noexcept;

		USE_RESULT maths::TransformMatrix<float> GetTransformationMatrix2D() const noexcept;
		
	private:
		maths::Vector3f position;
		maths::Vector3f rotations;
		maths::Vector3f scale;
	};
}
