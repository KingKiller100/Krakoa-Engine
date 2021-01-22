#pragma once

#include "../ScriptEntity.hpp"

#include "../../../Util/UniqueID.hpp"
#include "../../../Debug/Debug.hpp"

#include <Template/kToImpl.hpp>

#include <functional>
#include <unordered_map>

namespace krakoa::scene
{
	class Scene;

	namespace ecs::components
	{
		class NativeScriptComponent : protected util::TypeUniqueIdentifier<NativeScriptComponent>
		{
		public:
			template<typename Script>
			void Bind()
			{
				KRK_ASSERT(!HadScript<Script>()
					, "This script is already a part of this entity");

				scripts.emplace(GetUniqueID<Script>(), nullptr);
				
				initScriptFuncs.emplace_back([this]()
				{
					auto& script = scripts.at(GetUniqueID<Script>());
					script = Make_Solo<Script>();
				});
			}
			
			template<typename Script>
			Script& GetScript()
			{
				const auto uid = GetUniqueID<Script>();
				auto& script = scripts.at(uid);
				return klib::ToImpl<Script>(script);
			}
			
			template<typename Script>
			bool IsScriptActive()
			{
				return !scripts.empty() && scripts.at(GetUniqueID<Script>()) != nullptr;
			}
			
			bool IsActive() const;

			friend class Scene;

		private:
			void InvokeCreate(Entity* entity);
			void InvokeDestroy();
			void InvokeUpdate(float deltaTime);

			void SetEntity(Entity* entity);

		private:
			template<typename Script>
			bool HadScript()
			{
				return scripts.find(GetUniqueID<Script>()) != scripts.end();
			}
			
		private:
			std::unordered_map<ID_t, Solo_Ptr<ScriptEntity>> scripts;

			std::vector<std::function<void()>> initScriptFuncs;
		};
	}
}