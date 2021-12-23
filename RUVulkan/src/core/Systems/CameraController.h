#pragma once
#include "../Pipeline/Camera.h"
#include"../GameObject.h"
class CameraController
{
public:
	CameraController(GameObject& object) : obj(object) {}
	void Update(float frameTime);

private:
	GameObject& obj;
	glm::vec3 currentMouseVelocity{};
	glm::vec3 lastMouseVelocity{};
};