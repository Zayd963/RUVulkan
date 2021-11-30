#version 450

layout (location = 0) in vec2 position;
layout (location = 1) in vec4 color;

layout (location = 0) out vec4 v_Color;

void main()
{
	v_Color = color;
	gl_Position = vec4(position, 0.0, 1.0);
}