#include <Krakoa.hpp>
#include <Core/EntryPoint.hpp>

#include "Layers/Keditor2DLayer.hpp"

namespace krakoa
{
	class Keditor : public Application
	{
	public:
		Keditor()
			: Application("Keditor")
		{}

		~Keditor() override
			= default;

		void Initialize() override
		{
			Application::Initialize(); // Must be the first function call in this function

			PushLayer(new Keditor2DLayer());
		}

		void ShutDown() override
		{
			isRunning = false;
			Application::ShutDown(); // Must be the last function call in this function
		}
	};

	[[nodiscard]] krakoa::Application* CreateApplication()
	{
		return new Keditor();
	}
}
	