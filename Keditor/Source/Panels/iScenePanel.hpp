#pragma once

namespace krakoa
{
	namespace scene
	{
		class iScene;
		
		namespace panels
		{
			class iScenePanel
			{
			public:
				virtual ~iScenePanel() noexcept = default;
				virtual void LogConnected() const = 0;
				virtual void DrawActiveScene(const iScene& scene) = 0;
				virtual void DrawNoActiveScene() = 0;
			};
		}
	}
}
