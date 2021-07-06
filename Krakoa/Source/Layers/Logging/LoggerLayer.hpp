#pragma once
#include "../LayerBase.hpp"

namespace krakoa
{
	class LoggerLayer final : public LayerBase
	{
	public:
		LoggerLayer();
		~LoggerLayer();

		void OnAttach() override;
		void OnRender() override;

	private:
		void AddDestinaitonToEngineLogger();
	};

}
