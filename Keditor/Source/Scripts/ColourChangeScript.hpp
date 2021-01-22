#pragma once

#include <Krakoa.hpp>

#include <Maths/Vectors/Vector3.hpp>

namespace krakoa
{
	class ColourChangeScript : public scene::ecs::ScriptEntity
	{
	public:
		void SetColour(const kmaths::Vector4f& colour) const
		{
			appearance->SetColour(colour);
		}

	protected:
		void OnCreate() override
		{
			appearance = GetComponent<scene::ecs::components::Appearance2DComponent>();
		}

	private:
		scene::ecs::components::Appearance2DComponent* appearance = nullptr;
		kmaths::Vector4f geometryColour;
	};
}