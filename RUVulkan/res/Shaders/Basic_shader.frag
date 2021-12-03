#version 450

layout(location = 0) out vec4 o_Color;
layout(location = 0) in vec4 v_Color;
layout(push_constant) uniform Push
{
	mat4 transform;
	vec4 color;
}push;



void main()
{
	o_Color = v_Color;
}