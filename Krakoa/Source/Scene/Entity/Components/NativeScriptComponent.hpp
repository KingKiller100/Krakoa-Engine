#pragma once

#include "../ScriptEntity.hpp"

#include <functional>

namespace krakoa::scene::ecs::components
{
	struct NativeScriptComponent
	{
	public:
		template<typename Script>
		void Bind()
		{
			ctorFunc = [this]()
			{
				owner = new Script();
			};
			
			dtorFunc = [this]()
			{
				Script* script = (Script*)owner;
				delete script;
				owner = nullptr;
			};
			
			
			onCreateFunc = [](ScriptEntity* entity)
			{
				Script* script = (Script*)entity;
				script->OnCreate();
			};
			
			onDestroyFunc = [](ScriptEntity* entity)
			{
				Script* script = (Script*)entity;
				script->OnDestroy();
			};
			
			onUpdateFunc = [](ScriptEntity* entity, float time)
			{
				Script* script = (Script*)entity;
				script->OnUpdate(time);
			};
		}

		bool IsActive() const;

		void InvokeCreate();
		void InvokeDestroy();
		void InvokeUpdate(float deltaTime);
		
	private:
		ScriptEntity* owner = nullptr;

		std::function<void()> ctorFunc = nullptr;
		std::function<void()> dtorFunc = nullptr;
		
		std::function<void(ScriptEntity*)> onCreateFunc = nullptr;
		std::function<void(ScriptEntity*, float)> onUpdateFunc = nullptr;
		std::function<void(ScriptEntity*)> onDestroyFunc = nullptr;
	};
}
