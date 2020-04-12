#version 330 core

layout(location = 0) out vec4 out_color;

in vec3 v_Position;
in vec4 v_Colour;

void main()
{
	out_color = v_Colour;
}