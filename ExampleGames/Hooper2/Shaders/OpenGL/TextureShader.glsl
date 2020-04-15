#shader __Vertex__

#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

uniform mat4 u_VpMat;
uniform mat4 u_TransformMat;

out vec2 v_TexCoord;

void main()
{
	v_TexCoord = a_TexCoord;
	gl_Position = u_VpMat * u_TransformMat * vec4(a_Position, 1.0);
}

#shader __Fragment__

#version 330 core

layout(location = 0) out vec4 out_color;

in vec2  v_TexCoord;

uniform sampler2D u_Texture;

void main()
{
	out_color = texture(u_Texture, v_TexCoord);
}