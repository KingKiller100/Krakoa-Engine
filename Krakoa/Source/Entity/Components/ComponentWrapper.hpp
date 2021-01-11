#pragma once

#include "../EntityUID.hpp"

#include "../../Debug/Instrumentor.hpp"
#include "../../Core/EngineMacros.hpp"
#include "../../Core/PointerTypes.hpp"

namespace krakoa
{
	class ComponentWrapper
	{
	public:
		template<typename Component, typename ...Args, typename = std::enable_if_t<
			std::is_constructible_v<Component, Args...>
			>>
			ComponentWrapper(EntityUID entityUId, Args&& ...params)
			: active(true)
			, owner(entityUId)
		{
			SetComponent<Component, Args...>(entityUId, params);
		}

		~ComponentWrapper() noexcept;

		USE_RESULT bool IsActive() const;

		void Activate() noexcept;

		void Deactivate() noexcept;

		void SetOwner(EntityUID entityUId);

		template<typename Component>
		Component& GetComponent()
		{
			KRK_PROFILE_FUNCTION();
			auto*& comp = (Component*&)component.get();
			return *comp;
		}

		template<typename Component>
		const Component& GetComponent() const
		{
			KRK_PROFILE_FUNCTION();
			auto* comp = (Component*)component.get();
			return *comp;
		}

		template<typename Component, typename ...Args, typename = std::enable_if_t<
			std::is_constructible_v<Component, Args...>
			>>
			void SetComponent(EntityUID entity, Args&& ...params)
		{
			KRK_PROFILE_FUNCTION();
			Component* comp = new Component(std::forward<Args>(params)...);
			component.reset((void*)comp);
			SetOwner(entity);
		}

		constexpr ComponentWrapper(const ComponentWrapper&) = delete;
		constexpr ComponentWrapper& operator=(const ComponentWrapper&) = delete;

	private:
		Solo_Ptr<void> component;
		bool active;
		EntityUID owner;
	};
}
