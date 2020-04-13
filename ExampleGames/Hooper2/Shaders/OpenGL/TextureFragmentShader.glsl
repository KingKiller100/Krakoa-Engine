#version 330 core

layout(location = 0) out vec4 out_color;
in vec4 v_Colour;

in vec2  v_TexCoord;

void main()
{
	out_color = vec4(v_TexCoord, 0.0, 1.0);
}