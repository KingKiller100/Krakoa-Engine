#version 330 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_VpMat;
uniform mat4 u_TransformMat;

void main()
{
	gl_Position = u_VpMat * u_TransformMat * vec4(a_Position, 1.0);
}