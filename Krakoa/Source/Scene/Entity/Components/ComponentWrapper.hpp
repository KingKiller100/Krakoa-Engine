#pragma once

#include "../ECS_UID.hpp"

#include "../../../Debug/Instrumentor.hpp"
#include "../../../Core/EngineMacros.hpp"
#include "../../../Core/PointerTypes.hpp"

namespace krakoa
{
	class ComponentWrapper
	{
	public:
		using UID = ComponentUID;
	public:
		ComponentWrapper(UID compUid, EntityUID entityUId);

		ComponentWrapper(const ComponentWrapper& other) noexcept = delete;
		ComponentWrapper(ComponentWrapper&& other) noexcept;

		ComponentWrapper& operator=(const ComponentWrapper& other) noexcept = delete;
		ComponentWrapper& operator=(ComponentWrapper&& other) noexcept;

		~ComponentWrapper() noexcept;

		USE_RESULT bool IsActive() const;

		void Activate() noexcept;

		void Deactivate() noexcept;

		UID GetUID() const;

		EntityUID GetOwner() const;
		void SetOwner(EntityUID entityUId);

	private:
		bool active;
		EntityUID owner;
		const UID uid;
	};

	template <typename Component>
	class InternalComponent : public ComponentWrapper
	{
	public:
		using Component_t = Component;

	public:
		InternalComponent(UID compUid, EntityUID entityUId)
			: ComponentWrapper(compUid, entityUId)
			, component(nullptr)
		{}

		~InternalComponent() noexcept
			= default;

		template<typename ...Args>
		void Create(Args&&... params)
		{
			component.reset(new Component(std::forward<Args>(params)...));
		}

		Component& Ref()
		{
			return *component;
		}

		Component* Ptr()
		{
			return component.get();
		}

		const Component& Ref() const
		{
			return *component;
		}

	private:
		Solo_Ptr<Component> component;
	};

}
