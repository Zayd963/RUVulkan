#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;

layout (location = 0) out vec4 v_Color;

layout(push_constant) uniform Push
{
	mat4 transform;
	vec4 color;
}push;

void main()
{
	v_Color = color;
	gl_Position = push.transform * vec4(position, 1.0f);
}