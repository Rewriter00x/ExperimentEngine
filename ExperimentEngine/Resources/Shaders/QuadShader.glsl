#type vertex
#version 410 core
			
layout(location = 0) in vec4 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoord;
layout(location = 3) in int a_TexIndex;

uniform mat4 u_ViewProjection;

out vec4 v_Color;
out vec2 v_TexCoord;
flat out int v_TexIndex;

void main()
{
	v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    v_TexIndex = a_TexIndex;

	gl_Position = u_ViewProjection * a_Position;
}

#type fragment
#version 410 core

layout(location = 0) out vec4 o_Color;

uniform sampler2D u_Textures[16];

in vec4 v_Color;
in vec2 v_TexCoord;
flat in int v_TexIndex;

void main()
{
	o_Color = texture(u_Textures[v_TexIndex], v_TexCoord) * v_Color;
}
