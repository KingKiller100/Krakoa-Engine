#include  "Precompile.hpp"
#include "TransformComponent.hpp"

#include "../../../Debug/Instrumentor.hpp"
#include <Template/kTypeName.hpp>

namespace krakoa::components
{
	TransformComponent::TransformComponent() noexcept 
		: rotationAxes(0.f, 0.f, 1.f),
		radians(0),
		scale(1.f, 1.f, 1.f)
	{}

	TransformComponent::TransformComponent(const kmaths::Vector3f& position, const float radians,
		const kmaths::Vector3f& rotationAxes,
		const kmaths::Vector3f& scale) noexcept
		: position(position),
		rotationAxes(rotationAxes),
		radians(radians),
		scale(scale)
	{}

	TransformComponent::~TransformComponent() noexcept
		= default;

	kmaths::TransformMatrix<float> TransformComponent::GetTransformationMatrix2D() const noexcept
	{
		KRK_PROFILE_FUNCTION();

		return kmaths::TRS(position, radians, rotationAxes, scale);
	}

	void TransformComponent::SetScale(const kmaths::Vector2f & value)
	{
		KRK_PROFILE_FUNCTION();
		SetScale(kmaths::Vector3f{ value.x, value.y, 1.f });
	}

	const char* TransformComponent::GetType() const noexcept
	{
		KRK_PROFILE_FUNCTION();

		return klib::GetTypeName<TransformComponent>();
	}

	const kmaths::Vector3f& TransformComponent::GetPosition() const noexcept
	{
		return position;
	}

	void TransformComponent::SetPosition(const kmaths::Vector3f& value) noexcept
	{
		position = value;
	}

	const kmaths::Vector3f& TransformComponent::GetScale() const noexcept
	{
		return scale;
	}

	void TransformComponent::SetScale(const kmaths::Vector3f& value) noexcept
	{
		scale = value;
	}

	const kmaths::Vector3f& TransformComponent::GetRotationAxes() const noexcept
	{
		return rotationAxes;
	}

	void TransformComponent::SetRotationAxes(const kmaths::Vector3f& value) noexcept
	{
		rotationAxes = value;
	}

	const float& TransformComponent::GetRotation() const noexcept
	{
		return radians;
	}

	void TransformComponent::SetRotation(const float value) noexcept
	{
		radians = value;
	}
}
