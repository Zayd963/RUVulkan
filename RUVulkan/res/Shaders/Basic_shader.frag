#version 450

layout(location = 0) out vec4 o_Color;
layout(location = 0) in vec4 v_Color;
layout(push_constant) uniform Push
{
	mat4 normalMatrix;
	mat4 modelMatrix;
}push;


void main()
{
	o_Color = v_Color;
}