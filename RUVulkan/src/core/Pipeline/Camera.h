#pragma once
#define GLM_FORCE_RADIANS
#define FORCE_DEPTH_ZERO_TO_ONE
#include <glm.hpp>

class Camera
{
public:
	void SetOrthoProjection(float l, float r, float top, float bottom, float near, float far);
	void SetPerspectiveProjection(float fovy, float aspect, float near, float far);

	const glm::mat4& GetProjection() const { return projectionMatrix; };
	const glm::mat4& GetView() const { return viewMatrix; };

	void SetViewDirection(glm::vec3 pos, glm::vec3 dir, glm::vec3 up = glm::vec3{ 0.f, -1.f, 0.f });
	void SetViewTarget(glm::vec3 pos, glm::vec3 target, glm::vec3 up = glm::vec3{ 0.f, -1.f, 0.f });
	void SetViewYXZ(glm::vec3 pos, glm::vec3 rotation);
private:
	glm::mat4 projectionMatrix{ 1.0f };
	glm::mat4 viewMatrix{ 1.0f };

};

