#Shader __VERTEX__

#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Colour;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in float a_TexIdx;

uniform mat4 u_VpMat;

out vec4 v_Colour;
out float v_TexIdx;
out vec2 v_TexCoord;

void main()
{
	v_Colour = a_Colour;
	v_TexIdx = a_TexIdx;
	v_TexCoord = a_TexCoord;
	gl_Position = u_VpMat * vec4(a_Position, 1.0);
}

#Shader __FRAGMENT__

#version 330 core
#define GLSLIFY 1

layout(location = 0) out vec4 out_color;

in vec4 v_Colour;
in vec2 v_TexCoord;
in float v_TexIdx;

uniform sampler2D u_Textures[32];

void main()
{
	out_color = texture(u_Textures[int(v_TexIdx)], v_TexCoord) * v_Colour;
}

