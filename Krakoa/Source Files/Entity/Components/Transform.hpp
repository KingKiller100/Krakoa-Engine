#pragma once

#include "ComponentBase.hpp"

#include <Maths/Vectors/PredefinedVectors.hpp>
#include <Maths/Matrices/TransformMatrix.hpp>
#include <Maths/Matrices/MatrixMathsHelper.hpp>

namespace krakoa::components
{
	class Transform final : public ComponentBase
	{
	public:
		constexpr Transform() noexcept
			= default;
		
		constexpr Transform(const kmaths::Vector3f& position, 
			const float radians, 
			const kmaths::Vector3f& rotationAxes, 
			const kmaths::Vector3f& scale) noexcept
			: position(position),
			rotationAxes(rotationAxes),
			radians(radians),
			scale(scale)
		{}

		~Transform() noexcept override;

		USE_RESULT constexpr const kmaths::Vector3f& GetPosition() const noexcept { return position; }
		constexpr void SetPosition(const kmaths::Vector3f& value) noexcept { (position) = value; }

		USE_RESULT constexpr const kmaths::Vector3f& GetScale() const noexcept { return scale; }
		constexpr void SetScale(const kmaths::Vector3f& value) noexcept { (scale) = value; }
		void SetScale(const kmaths::Vector2f& value);

		USE_RESULT constexpr const kmaths::Vector3f& GetRotationAxes() const noexcept { return rotationAxes; }
		constexpr void SetRotationAxes(const kmaths::Vector3f& value) noexcept { (rotationAxes) = value; };

		USE_RESULT constexpr const float& GetRotation() const noexcept { return radians; }
		constexpr void SetRotation(const float value) noexcept { (radians) = value; }

		USE_RESULT constexpr kmaths::TransformMatrix<float> GetTransformationMatrix2D() const noexcept
		{
			KRK_PROFILE_FUNCTION();

			return kmaths::TRS(position, radians, rotationAxes, scale);
		}

		STATIC_GET_COMPONENT_TYPE(Transform)
		USE_RESULT const char* GetType() const noexcept override;
		
	private:
		kmaths::Vector3f position{};
		kmaths::Vector3f rotationAxes{ 0.f, 0.f, 1.f };
		float radians = 0.f;
		kmaths::Vector3f scale{ 1.f, 1.f, 1.f };
	};
}
