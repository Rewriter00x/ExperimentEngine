#type vertex
#version 410 core
			
layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Color;

uniform mat4 u_ViewProjection;

out vec4 v_Color;

void main()
{
	v_Color = a_Color;

	gl_Position = u_ViewProjection * a_Position;
}

#type fragment
#version 410 core

layout(location = 0) out vec4 o_Color;

in vec4 v_Color;

void main()
{
	o_Color = v_Color;
}