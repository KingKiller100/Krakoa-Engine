#version 330 core

layout(location = 0) in vec3 a_Position;

out vec3 v_Position;
out vec4 v_Colour;

uniform mat4 u_VpMat;
uniform mat4 u_TransformMat;
uniform vec4 u_Colour;

void main()
{
	v_Colour = u_Colour;
	v_Position = a_Position;
	gl_Position = u_VpMat * u_TransformMat * vec4(a_Position, 1.0);
}