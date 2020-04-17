#Shader __VERTEX__

#version 330 core

layout(location = 0) in vec3 a_Position;

uniform mat4 u_VpMat;
uniform mat4 u_TransformMat;

void main()
{
	gl_Position = u_VpMat * u_TransformMat * vec4(a_Position, 1.0);
}

#Shader __FRAGMENT__

#version 330 core

layout(location = 0) out vec4 out_Colour;

uniform vec4 u_Colour;

void main()
{
	out_Colour = u_Colour;
}