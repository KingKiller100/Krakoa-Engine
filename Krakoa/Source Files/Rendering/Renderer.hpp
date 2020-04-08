#pragma once

#include "iRendererAPI.hpp"

#include "../Patterns/SimpleSingleton.hpp"

namespace krakoa::graphics
{

	class Renderer : public pattern::SimpleSingleton<Renderer>
	{
	public:
		Renderer(Token&&);
		~Renderer();
		 
		void BeginScene() const;
		void EndScene() const;
		void Submit(const iVertexArray& vertexArray) const;

		inline iRendererAPI::API GetAPI() const noexcept { return iRendererAPI::GetAPI(); }

	};
}