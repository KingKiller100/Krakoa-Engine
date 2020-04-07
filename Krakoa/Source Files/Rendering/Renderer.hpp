#pragma once

#include "../Patterns/SimpleSingleton.hpp"

namespace krakoa::graphics
{
	enum class RendererAPI : unsigned short
	{
		NONE,
		OPENGL,
	};

	class Renderer : public pattern::SimpleSingleton<Renderer>
	{
	public:
		Renderer(Token&&);
		~Renderer();

		inline RendererAPI GetAPI() const noexcept { return rendererAPI; }

	private:
		RendererAPI rendererAPI;
	};
}