#pragma once

#include "../ScriptEntity.hpp"

#include <Template/kToImpl.hpp>

#include <functional>

namespace krakoa::scene
{
	class Scene;

	namespace ecs::components
	{
		class NativeScriptComponent
		{
		public:
			template<typename Script>
			Script& GetScript()
			{
				return klib::ToImpl<Script>(owner);
			}

			template<typename Script>
			void Bind()
			{
				initScriptFunc = [this]()
				{
					owner = new Script();
				};

				destroyScriptFunc = [this]()
				{
					delete owner;
					owner = nullptr;
				};
			}

			friend class Scene;

		private:
			bool IsActive() const;

			void InvokeCreate(Entity* entity);
			void InvokeDestroy();
			void InvokeUpdate(float deltaTime);

			void SetEntity(Entity* entity);

		private:
			ScriptEntity* owner = nullptr;

			std::function<void()> initScriptFunc = nullptr;
			std::function<void()> destroyScriptFunc = nullptr;
		};
	}
}