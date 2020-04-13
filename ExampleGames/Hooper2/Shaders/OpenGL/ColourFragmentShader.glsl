#version 330 core

layout(location = 0) out vec4 out_Colour;

uniform vec4 u_Colour;

void main()
{
	out_Colour = u_Colour;
}