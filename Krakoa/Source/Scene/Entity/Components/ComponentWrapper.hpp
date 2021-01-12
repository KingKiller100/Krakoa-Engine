#pragma once

#include "../ECS_UID.hpp"

#include "../../../Debug/Instrumentor.hpp"
#include "../../../Core/EngineMacros.hpp"
#include "../../../Core/PointerTypes.hpp"

namespace krakoa
{
	class ComponentWrapper
	{
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

		template<typename Component>
		Component& GetComponent()
		{
			KRK_PROFILE_FUNCTION();
			auto* comp = (Component*)component.get();
			return *comp;
		}

		template<typename Component>
		const Component& GetComponent() const
		{
			KRK_PROFILE_FUNCTION();
			auto* comp = (Component*)component.get();
			return *comp;
		}

		template<typename Component, typename ...Args>
		void SetComponent(Args&& ...params)
		{
			KRK_PROFILE_FUNCTION();
			
			auto* comp = new Component(std::forward<Args>(params)...);
			component.reset((uintptr_t*)comp);
		}

	private:
		Solo_Ptr<uintptr_t> component;
		bool active;
		EntityUID owner;
		const UID uid;
	};
}
