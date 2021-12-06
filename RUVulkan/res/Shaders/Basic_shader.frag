#version 450

layout(location = 0) out vec4 o_Color;
layout (location = 0) in vec4 v_Color;
layout (location = 1) in vec3 v_FragPosWorld;
layout (location = 2) in vec3 v_FragNormalWorld;

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
	vec3 directionToLight = ubo.lightDirection - v_FragPosWorld.xyz;
	float attenuation = 1.0f / dot(directionToLight, directionToLight);

	vec3 lightColor = ubo.lightColor.xyz * ubo.lightColor.w * attenuation;
	vec3 ambientLight = ubo.ambinentLightColor.xyz * ubo.ambinentLightColor.w;
	vec3 diffuseLight = lightColor * max(dot(normalize(v_FragNormalWorld), normalize(ubo.lightDirection)), 0);
	o_Color = vec4((diffuseLight + ambientLight) * v_Color.xyz, 1.0f);
}