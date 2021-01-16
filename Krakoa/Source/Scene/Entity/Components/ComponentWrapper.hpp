#pragma once

#include "../ECS_UID.hpp"

#include "../../../Core/EngineMacros.hpp"

namespace krakoa::scene::ecs
{
	class ComponentWrapperBase
	{
	public:
		using UID = ComponentUID;
	public:
		ComponentWrapperBase(UID compUid, EntityUID entityUId);

		ComponentWrapperBase(const ComponentWrapperBase& other) noexcept = delete;
		ComponentWrapperBase& operator=(const ComponentWrapperBase& other) noexcept = delete;

		ComponentWrapperBase(ComponentWrapperBase&& other) noexcept;
		ComponentWrapperBase& operator=(ComponentWrapperBase&& other) noexcept;

		~ComponentWrapperBase() noexcept;

		USE_RESULT bool IsActive() const;

		void Activate() noexcept;

		void Deactivate() noexcept;

		[[nodiscard]] UID GetUID() const;

		[[nodiscard]] EntityUID GetOwner() const;
		void SetOwner(EntityUID entityUId);

	private:
		bool active;
		EntityUID owner;
		const UID uid;
	};

	template <typename Component>
	class ComponentWrapper : public ComponentWrapperBase
	{
	public:
		using Component_t = Component;

	public:
		template<typename ...Args>
		constexpr ComponentWrapper(UID compUid, EntityUID entityUId, Args&&... params)
			: ComponentWrapperBase(compUid, entityUId)
			, component(std::forward<Args>(params)...)
		{}

		~ComponentWrapper() noexcept
			= default;

		constexpr Component& Ref()
		{
			return component;
		}

		constexpr Component* Ptr()
		{
			return std::addressof(component);
		}

		constexpr const Component& Ref() const
		{
			return component;
		}

		constexpr operator Component&() const noexcept
		{
			return Ref();
		}

		constexpr operator Component&() noexcept
		{
			return Ref();
		}

	private:
		Component component;
	};
}
