#include "Precompile.hpp"
#include "SceneManager.hpp"

#include "Scene.hpp"

#include "../Graphics/2D/Renderer2D.hpp"

#include "Entity/Components/AppearanceComponent.hpp"
#include "Entity/Components/CameraComponent.hpp"
#include "Entity/Components/TransformComponent.hpp"

namespace krakoa::scene
{
	using namespace ecs::components;

	SceneManager::SceneManager()
		: entityComponentSystem(new ecs::EntityComponentSystem())
	{
	}

	SceneManager::~SceneManager()
	{
		RemoveAll();
		entityComponentSystem->RemoveAllEntities();
	}

	void SceneManager::Add(const std::string_view& name)
	{
		auto* scene = new Scene(name, entityComponentSystem);
		scenes[name.data()].reset(scene);
		currentScene = name;
		scene->OnLoad();
	}

	bool SceneManager::Remove(const std::string_view& name)
	{
		const auto iter = scenes.find(name.data());
		if (iter == scenes.end())
			return false;

		scenes.erase(iter);
		return true;
	}

	void SceneManager::RemoveAll()
	{
		scenes.clear();
	}

	iScene& SceneManager::GetCurrentScene()
	{
		return *scenes.at(currentScene);
	}

	void SceneManager::LoadFromFile(const std::filesystem::path& path)
	{
	}

	void SceneManager::OnUpdate(const float deltaTime)
	{
		auto& scene = GetCurrentScene();
		scene.OnUpdate(deltaTime);
		DrawEntities(scene);
	}

	void SceneManager::DrawEntities(const iScene& scene) const
	{
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
				KRK_FATAL("Failed to draw unknown geometry type");
				break;
			}
		}

		graphics::Renderer2D::EndScene();
	}
}
