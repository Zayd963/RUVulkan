#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;
layout (location = 2) in vec3 normal;
layout (location = 3) in vec2 uv;

layout (location = 0) out vec4 v_Color;
layout (location = 1) out vec3 v_FragPosWorld;
layout (location = 2) out vec3 v_FragNormalWorld;

layout (set = 0, binding = 0) uniform GlobalUBO
{
	mat4 projectionView;
	vec4 ambinentLightColor;
	vec3 lightDirection;
	vec4 lightColor;
}ubo;

layout(push_constant) uniform Push
{
	mat4 normalMatrix;
	mat4 modelMatrix;
}push;

void main()
{
	vec4 positionWorld = push.modelMatrix * vec4(position, 1.0f);

	gl_Position = ubo.projectionView * push.modelMatrix * vec4(position, 1.0f);
	
	v_FragNormalWorld = normalize(mat3(push.normalMatrix) * normal);
	v_FragPosWorld = positionWorld.xyz;
	v_Color = color;
}