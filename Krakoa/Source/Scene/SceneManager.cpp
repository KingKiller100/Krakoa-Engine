#include "Precompile.hpp"
#include "SceneManager.hpp"

#include "Scene.hpp"
#include "Serialization/SceneSerializer.hpp"

#include "../Graphics/2D/Renderer2D.hpp"
#include "../FileSystem/VirtualFileExplorer.hpp"

#include "Entity/Components/CameraComponent.hpp"
#include "Entity/Components/TransformComponent.hpp"
#include "Entity/Components/AppearanceComponent.hpp"
#include "Entity/Components/NativeScriptComponent.hpp"

namespace krakoa::scene
{
	using namespace ecs::components;

	SceneManager::SceneManager()
		: SimpleStateMachine(SceneRuntimeState::STOP,
			[this](const auto& current, const auto& next)
	{
		const SceneRuntimeState prev = current;
		ResolveNextState(next);
		const auto msg = klib::ToString("Scene Manager state changed: {0} -> {1}",
			prev
			, state);
		KRK_NRM(msg);
	})
		, currentScene(scenes.end())
		, entityComponentSystem(new ecs::EntityComponentSystem())
		, state(SceneRuntimeState::STOP)
		, serializer(Make_Solo<serialization::SceneSerializer>())
	{
		//Temporary
		filesystem::VirtualFileExplorer::Mount("Keditor\\Assets\\Scenes", "Scenes");
	}

	SceneManager::~SceneManager()
	{
		RemoveAll();
		entityComponentSystem->RemoveAllEntities();
	}

	Weak_Ptr<iScene> SceneManager::Add(const std::string& name)
	{
		KRK_PROFILE_FUNCTION();

		auto& scene = scenes[name];
		scene.reset(new Scene(name, entityComponentSystem));
		scene->SetRuntimeState(std::addressof(state));
		scene->OnLoad();
		currentScene = scenes.find(name);
		serializer->SetScene(scene);
		return scene;
	}

	bool SceneManager::Remove(std::string_view name)
	{
		KRK_PROFILE_FUNCTION();

		const auto iter = scenes.find(name.data());
		if (iter == scenes.end())
			return false;

		iter->second->Clear();
		scenes.erase(iter);
		return true;
	}

	void SceneManager::RemoveAll()
	{
		KRK_PROFILE_FUNCTION();
		for (auto&& scnInfo : scenes)
		{
			const auto& scene = scnInfo.second;
			if (scene.use_count() > 1)
			{
				const auto msg = util::Fmt("Scene \"{0}\" is still active", scene->GetName());
				KRK_WRN(msg);
			}
			scene->Clear();
		}
		scenes.clear();
	}

	Weak_Ptr<iScene> SceneManager::GetCurrentScene() const
	{
		KRK_PROFILE_FUNCTION();
		return currentScene->second;
	}

	Weak_Ptr<iScene> SceneManager::GetCurrentScene()
	{
		KRK_PROFILE_FUNCTION();
		return currentScene->second;
	}

	void SceneManager::SetCurrentScene(const std::string& name)
	{
		currentScene = scenes.find(name);
	}

	void SceneManager::SaveToFile(const std::filesystem::path& path)
	{
		KRK_INF(util::Fmt("Saving scene to \"{0}\"", path));
		serializer->Serialize(path);
	}

	void SceneManager::LoadFromFile(const std::filesystem::path& path)
	{
		KRK_PROFILE_FUNCTION();
		const auto sceneName = path.stem().string();
		if (HasScene(sceneName))
		{
			currentScene = scenes.find(sceneName);
			return;
		}

		const auto scn = Add(sceneName);
		serializer->SetScene(scn);
		KRK_INF(util::Fmt("Loading scene from \"{0}\"", path));
		serializer->Deserialize(path);
	}

	void SceneManager::OnUpdate(const float deltaTime)
	{
		KRK_PROFILE_FUNCTION();
		auto& scene = *currentScene->second;

		scene.OnUpdate(deltaTime);

		RenderEntities(scene);
	}

	bool SceneManager::HasScene(std::string_view sceneName) const
	{
		const auto iter = std::find_if(scenes.begin(), scenes.end(), [sceneName]
		(const decltype(scenes)::value_type& scene)
		{
			return scene.second->GetName() == sceneName;
		});

		return iter != scenes.cend();
	}

	bool SceneManager::HasActiveScene() const
	{
		return currentScene != scenes.end();
	}

	void SceneManager::TogglePlayScene()
	{
		if (GetState() == SceneRuntimeState::PLAY)
			SetState(SceneRuntimeState::STOP);
		else
			SetState(SceneRuntimeState::PLAY);
	}

	void SceneManager::StopScene()
	{
		SetState(SceneRuntimeState::STOP);
	}

	void SceneManager::ResolveNextState(SceneRuntimeState::Underlying_t nextState)
	{
		state = nextState;
	}

	void SceneManager::RenderEntities(const iScene& scene) const
	{
		KRK_PROFILE_FUNCTION();

		if (scene.Empty())
			return;

		iCamera* camera = nullptr;
		auto cameraTransform = maths::GetTransformIdentity<float>();

		const auto cameraEntities
			= entityComponentSystem->GetEntitiesWithComponents<CameraComponent, TransformComponent>();

		for (const auto id : cameraEntities)
		{
			const auto& entity = scene.GetEntity(id);

			const auto& cam = entity.GetComponent<CameraComponent>();
			auto& tfm = entity.GetComponent<TransformComponent>();

			if (!cam.IsPrimary())
				continue;

			camera = std::addressof(cam.GetCamera());
			cameraTransform = tfm.GetTransformationMatrix2D();
			break;
		}

		if (!camera)
			return;

		const auto drawables
			= entityComponentSystem->GetEntitiesWithComponents<Appearance2DComponent, TransformComponent>();

		gfx::Renderer2D::BeginScene(*camera, cameraTransform);

		for (const auto id : drawables)
		{
			const auto& entity = scene.GetEntity(id);

			const auto& appearance = entity.GetComponent<Appearance2DComponent>();
			const auto& transform = entity.GetComponent<TransformComponent>();

			switch (appearance.GetGeometryType()) {
			case gfx::GeometryType::QUAD:
				gfx::Renderer2D::DrawQuad(appearance, transform);
				break;

			case gfx::GeometryType::TRIANGLE:
				gfx::Renderer2D::DrawTriangle(appearance, transform);
				break;

			case gfx::GeometryType::CIRCLE:
				//	gfx::Renderer2D::DrawCircle(appearance, transform);
				break;

			default: // case of an unknown geometry type
				KRK_FATAL(klib::ToString("Failed to draw entity {0} - unknown geometry type", entity.GetID()));
				break;
			}
		}

		gfx::Renderer2D::EndScene();
	}
}
