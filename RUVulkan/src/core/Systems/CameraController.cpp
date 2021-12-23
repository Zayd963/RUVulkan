#include "CameraController.h"
#include "glm.hpp"
#include"../Input.h"
void CameraController::Update(float frameTime)
{
	float yaw = obj.transform.rotation.y;
	const glm::vec3 forwardDir{ sinf(yaw), 0.f, cosf(yaw) };
	const glm::vec3 rightDir{ forwardDir.z, 0.f, -forwardDir.x };
	const glm::vec3 upDir{ 0.f, -1.f, 0.f };

	glm::vec3 moveDir{ 0 };
	if (Input::isKey(SDL_SCANCODE_D))
		moveDir += rightDir;
	if (Input::isKey(SDL_SCANCODE_A))
		moveDir -= rightDir;
	if (Input::isKey(SDL_SCANCODE_W))
		moveDir += forwardDir;
	if (Input::isKey(SDL_SCANCODE_S))
		moveDir -= forwardDir;
	if (Input::isKey(SDL_SCANCODE_Q))
		moveDir -= upDir;
	if (Input::isKey(SDL_SCANCODE_E))
		moveDir += upDir;

	if (glm::dot(moveDir, moveDir) > std::numeric_limits<float>::epsilon())
		obj.transform.translation += 5.f * frameTime* glm::normalize(moveDir);

	currentMouseVelocity = { (float)Input::GetMouseX(), (float)Input::GetMouseY(), 0 };

	glm::vec3 mouseVelocity = (currentMouseVelocity - lastMouseVelocity) / frameTime;

	auto temp = glm::vec2(mouseVelocity.y, mouseVelocity.x);

	if (Input::isMouseHeld(SDL_BUTTON_RMASK))
	{
		obj.transform.rotation.x += (temp.x * frameTime) / 1000;
		obj.transform.rotation.y += -(temp.y * frameTime) / 1000;
	}

	lastMouseVelocity = currentMouseVelocity;
}



