#include "Precompile.hpp"
#include "SceneManager.hpp"

#include "Scene.hpp"

#include "../Graphics/2D/Renderer2D.hpp"

#include "Entity/Components/CameraComponent.hpp"
#include "Entity/Components/TransformComponent.hpp"
#include "Entity/Components/AppearanceComponent.hpp"
#include "Entity/Components/NativeScriptComponent.hpp"

namespace krakoa::scene
{
	using namespace ecs::components;

	SceneManager::SceneManager()
		: SimpleStateMachine(SceneRuntimeState::STOP, 
		     [](const auto& current, const auto& next)
		     {
			 const auto msg = klib::ToString("Scene Manager state changed: {0} -> {1}",
			 current
			 , next);
			 KRK_INF(msg);
		     })
		, currentScene(scenes.end())
		, entityComponentSystem(new ecs::EntityComponentSystem())
	{}

	SceneManager::~SceneManager()
	{
		RemoveAll();
		entityComponentSystem->RemoveAllEntities();
	}

	void SceneManager::Add(const std::string& name)
	{
		KRK_PROFILE_FUNCTION();

		auto* scene = new Scene(name, entityComponentSystem);
		scenes[name].reset(scene);
		scene->SetRuntimeState((State_t*)std::addressof(GetState()));
		currentScene = scenes.find(name);
		scene->OnLoad();
	}

	bool SceneManager::Remove(const std::string_view& name)
	{
		KRK_PROFILE_FUNCTION();

		const auto iter = scenes.find(name.data());
		if (iter == scenes.end())
			return false;

		scenes.erase(iter);
		return true;
	}

	void SceneManager::RemoveAll()
	{
		KRK_PROFILE_FUNCTION();
		scenes.clear();
	}

	iScene& SceneManager::GetCurrentScene() const
	{
		KRK_PROFILE_FUNCTION();
		return *currentScene->second;
	}

	void SceneManager::SetCurrentScene(const std::string& name)
	{
		currentScene = scenes.find(name);
	}

	void SceneManager::LoadFromFile(const std::filesystem::path& path)
	{
		KRK_PROFILE_FUNCTION();
	}

	void SceneManager::OnUpdate(const float deltaTime)
	{
		KRK_PROFILE_FUNCTION();
		auto& scene = GetCurrentScene();

		scene.OnUpdate(deltaTime);

		RenderEntities(scene);
	}

	bool SceneManager::HasActiveScene() const
	{
		return currentScene != scenes.end();
	}

	void SceneManager::RenderEntities(const iScene& scene) const
	{
		KRK_PROFILE_FUNCTION();

		iCamera* camera = nullptr;
		kmaths::TransformMatrix<float> cameraTransform = kmaths::GetTransformIdentity<float>();

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

		graphics::Renderer2D::BeginScene(*camera, cameraTransform);

		for (const auto id : drawables)
		{
			const auto& entity = scene.GetEntity(id);

			const auto& appearance = entity.GetComponent<Appearance2DComponent>();
			const auto& transform = entity.GetComponent<TransformComponent>();

			switch (appearance.GetGeometryType()) {
			case graphics::GeometryType::QUAD:
				graphics::Renderer2D::DrawQuad(appearance, transform);
				break;

			case graphics::GeometryType::TRIANGLE:
				graphics::Renderer2D::DrawTriangle(appearance, transform);
				break;

			case graphics::GeometryType::CIRCLE:
				//	graphics::Renderer2D::DrawCircle(appearance, transform);
				break;

			default: // case of an unknown geometry type
				KRK_FATAL(klib::ToString("Failed to draw entity {0} - unknown geometry type", entity.GetID()));
				break;
			}
		}

		graphics::Renderer2D::EndScene();
	}
}
