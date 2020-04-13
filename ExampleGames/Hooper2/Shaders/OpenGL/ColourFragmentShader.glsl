#version 330 core

layout(location = 0) out vec4 out_Colour;

in vec4 v_Colour;

void main()
{
	out_Colour = v_Colour;
}