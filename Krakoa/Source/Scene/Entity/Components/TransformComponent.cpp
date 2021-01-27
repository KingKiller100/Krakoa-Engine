#include  "Precompile.hpp"
#include "TransformComponent.hpp"

#include "../../../Debug/Instrumentor.hpp"

#include <Maths/Quaternions/Quaternions.hpp>

namespace krakoa::scene::ecs::components
{
	TransformComponent::TransformComponent() noexcept
		: rotations(0.f, 0.f, 1.f),
		scale(1.f, 1.f, 1.f)
	{}

	TransformComponent::TransformComponent(const kmaths::Vector3f& position,
		const kmaths::Vector3f& rotationAxes,
		const kmaths::Vector3f& scale) noexcept
		: position(position),
		rotations(rotationAxes),
		scale(scale)
	{}

	TransformComponent::~TransformComponent() noexcept
		= default;

	kmaths::TransformMatrix<float> TransformComponent::GetTransformationMatrix2D() const noexcept
	{
		KRK_PROFILE_FUNCTION();

		const auto quaternions = kmaths::Quaternionf::EulerToQuaternions(rotations);
		auto transform = quaternions.CalculateTransformMatrix(position)
			* kmaths::Scale(scale);
		return transform;
		// return kmaths::TRS(position, radians, rotations, scale);
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
}
