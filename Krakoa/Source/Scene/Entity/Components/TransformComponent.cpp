#include  "Precompile.hpp"
#include "TransformComponent.hpp"

#include "../../../Debug/Instrumentor.hpp"

#include <Maths/Quaternions/Quaternions.hpp>

#include "Maths/Matrices/MatrixMathsHelper.hpp"

namespace krakoa::scene::ecs::components
{
	TransformComponent::TransformComponent() noexcept
		: rotations(0.f, 0.f, 0.f),
		scale(1.f, 1.f, 1.f)
	{}

	TransformComponent::TransformComponent(const kmaths::Vector3f& position,
		const kmaths::Vector3f& rotation,
		const kmaths::Vector3f& scale) noexcept
		: position(position),
		rotations(rotation),
		scale(scale)
	{}

	TransformComponent::~TransformComponent() noexcept
		= default;

	kmaths::TransformMatrix<float> TransformComponent::GetTransformationMatrix2D() const noexcept
	{
		KRK_PROFILE_FUNCTION();

		//return kmaths::TRS(position, rotations.z, { 1, 1,1 },  scale);
		const auto quaternions = kmaths::Quaternionf::EulerToQuaternions(rotations);
		auto transform = quaternions.CalculateTransformMatrix(position)
			* kmaths::Scale(scale);
		return transform;
	}

	kmaths::TransformMatrix<float> TransformComponent::GetTransformationMatrix() const noexcept
	{
		KRK_PROFILE_FUNCTION();

		return kmaths::TRS(position, rotations.z, { 1.f, 1.f, 1.f }, scale);
	}

	void TransformComponent::SetScale(const kmaths::Vector2f& value)
	{
		KRK_PROFILE_FUNCTION();
		SetScale(kmaths::Vector3f{ value.x, value.y, 1.f });
	}

	kmaths::Vector3f TransformComponent::GetPosition() const noexcept
	{
		return position;
	}

	void TransformComponent::SetPosition(const kmaths::Vector3f& value) noexcept
	{
		position = value;
	}

	kmaths::Vector3f TransformComponent::GetScale() const noexcept
	{
		return scale;
	}

	void TransformComponent::SetScale(const kmaths::Vector3f& value) noexcept
	{
		scale = value;
	}

	kmaths::Vector3f TransformComponent::GetRotation(kmaths::Theta theta) const noexcept
	{
		return theta == kmaths::Theta::RADIANS
			? rotations
			: rotations * kmaths::constants::RadsToDegs<float>;
	}

	kmaths::Vector3f TransformComponent::GetRotation() const noexcept
	{
		return rotations;
	}

	void TransformComponent::SetRotation(const kmaths::Vector3f& value) noexcept
	{
		rotations = value;
	}

	void TransformComponent::SetRotationX(const kmaths::Vector3f::Type& value) noexcept
	{
		rotations.x = value;
	}

	void TransformComponent::SetRotationY(const kmaths::Vector3f::Type& value) noexcept
	{
		rotations.y = value;
	}

	void TransformComponent::SetRotationZ(const kmaths::Vector3f::Type& value) noexcept
	{
		rotations.z = value;
	}
}
