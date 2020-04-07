#pragma once

namespace krakoa::graphics
{

	class iShader
	{
	public:
		virtual void Bind() = 0;
		virtual void Unbind() = 0;
	};
}