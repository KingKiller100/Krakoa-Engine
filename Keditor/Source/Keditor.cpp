#include <Krakoa.hpp>
#include <Core/EntryPoint.hpp>

#include "Layers/Keditor2DLayer.hpp"

namespace krakoa
{
	class Keditor : public Application
	{
	public:
		Keditor(Token&& t)
			: Application(t, "Keditor")
		{
			INIT_LOGS("Keditor");
			TOGGLE_CONSOLE_LOGGING();
			SET_LOG_MIN(LOG_LVL_DBG);
		}

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

	void krakoa::CreateApplication()
	{
		Application::Create<Keditor>();
	}
}
	