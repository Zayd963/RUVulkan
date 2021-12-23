#include "Engine.h"
#include <iostream>
#include <array>
#include "Systems/SimpleRenderSystem.h"
#define GLM_FORCE_RADIANS
#define FORCE_DEPTH_ZERO_TO_ONE
#include <glm.hpp>
#include <gtc/constants.hpp>
#include "Pipeline/Camera.h"
#include "Input.h"
#include "Pipeline/Buffer.h"



void MoveInPlaneZ(float deltaTime, GameObject& gameObject)
{
	glm::vec3 rotate{ 0 };
	if (Input::isKey(SDL_SCANCODE_RIGHT))
		rotate.y += 1.f;
	if (Input::isKey(SDL_SCANCODE_LEFT))
		rotate.y -= 1.f;
	if (Input::isKey(SDL_SCANCODE_UP))
		rotate.x += 1.f;
	if (Input::isKey(SDL_SCANCODE_DOWN))
		rotate.x -= 1.f;
	if (glm::dot(rotate, rotate) > std::numeric_limits<float>::epsilon())
		gameObject.transform.rotation += 5.f * deltaTime * glm::normalize(rotate);

	gameObject.transform.rotation.x = glm::clamp(gameObject.transform.rotation.x, -1.5f, 1.5f);
	gameObject.transform.rotation.y = glm::mod(gameObject.transform.rotation.y, glm::two_pi<float>());

	float yaw = gameObject.transform.rotation.y;
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
		gameObject.transform.translation += 5.f * deltaTime * glm::normalize(moveDir);
}


bool Engine::Init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		return false;	
	return true;
}

void Engine::Run()
{
	
	auto currentTime = std::chrono::high_resolution_clock::now();

	Scene* scene = new Scene( device, window, renderer );

	while (Input::isRunning())
	{
		Input::Listen(renderer);
		
		auto newTime = std::chrono::high_resolution_clock::now();

		float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
		currentTime = newTime;

	
	
		scene->Update(frameTime);
		scene->Render();
	}
	vkDeviceWaitIdle(device.device());
	delete scene;
}

void Engine::Shutdown()
{
	SDL_Quit();
}



