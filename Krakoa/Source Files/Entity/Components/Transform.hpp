#pragma once

#include "ComponentBase.hpp"

#include <Maths/Vectors/PredefinedVectors.hpp>
#include <Maths/Matrices/TransformMatrix.hpp>
#include <Maths/Matrices/MatrixMathsHelper.hpp>

namespace krakoa::component
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

		~Transform() noexcept override
			= default;

		CONST_GETTER_CONSTEXPR(kmaths::Vector3f, GetPosition, position)
		SETTER_CONSTEXPR(kmaths::Vector3f, SetPosition, position)

		CONST_GETTER_CONSTEXPR(kmaths::Vector3f, GetScale, scale)
		SETTER_CONSTEXPR(kmaths::Vector3f, SetScale, scale)

		CONST_GETTER_CONSTEXPR(kmaths::Vector3f, GetRotationAxes, rotationAxes)
		SETTER_CONSTEXPR(kmaths::Vector3f, SetRotationAxes, rotationAxes);

		CONST_GETTER_CONSTEXPR(float, GetRotation, radians)
		USE_RESULT constexpr void SetRotation(const float value) noexcept { (radians) = value; }

		USE_RESULT constexpr kmaths::TransformMatrix<float> GetTransformationMatrix2D() const noexcept
		{
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
