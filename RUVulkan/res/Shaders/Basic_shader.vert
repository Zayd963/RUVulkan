#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 uv;

layout (location = 0) out vec4 v_Color;

layout(push_constant) uniform Push
{
	mat4 normalMatrix;
	mat4 modelMatrix;
}push;

const vec3 DIRECTION_TO_LIGHT = normalize(vec3(1.0, -3.0, -1.0));
const float AMBIENT = 0.2;
void main()
{
	
	gl_Position = push.modelMatrix * vec4(position, 1.0f);

	

	
	vec3 normalWorldSpace = normalize(mat3(push.normalMatrix) * normal);

	float lightIntesity = AMBIENT + max(dot(normalWorldSpace, DIRECTION_TO_LIGHT), 0);

	v_Color = lightIntesity * color;
}