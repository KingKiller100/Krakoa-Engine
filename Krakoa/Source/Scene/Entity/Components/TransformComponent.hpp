#pragma once

#include <Maths/kMathsTheta.hpp>
#include <Maths/Vectors/PredefinedVectors.hpp>
#include <Maths/Matrices/TransformMatrix.hpp>
#include <Maths/Matrices/MatrixMathsHelper.hpp>

namespace krakoa::scene::ecs::components
{
	class TransformComponent
	{
	public:
		TransformComponent() noexcept;

		TransformComponent(const kmaths::Vector3f& position,
		                   const float radians,
		                   const kmaths::Vector3f& rotationAxes,
		                   const kmaths::Vector3f& scale = {1.f, 1.f, 1.f}) noexcept;

		~TransformComponent() noexcept;

		USE_RESULT const kmaths::Vector3f& GetPosition() const noexcept;
		void SetPosition(const kmaths::Vector3f& value) noexcept;

		USE_RESULT const kmaths::Vector3f& GetScale() const noexcept;
		void SetScale(const kmaths::Vector3f& value) noexcept;
		void SetScale(const kmaths::Vector2f& value);

		USE_RESULT const kmaths::Vector3f& GetRotationAxes() const noexcept;
		void SetRotationAxes(const kmaths::Vector3f& value) noexcept;;

		USE_RESULT float GetRotation(kmaths::Theta theta = kmaths::Theta::RADIANS) const noexcept;
		void SetRotation(const float value) noexcept;

		USE_RESULT kmaths::TransformMatrix<float> GetTransformationMatrix2D() const noexcept;

		USE_RESULT const char* GetType() const noexcept;
		
	private:
		kmaths::Vector3f position;
		kmaths::Vector3f rotationAxes;
		float radians;
		kmaths::Vector3f scale;
	};
}
