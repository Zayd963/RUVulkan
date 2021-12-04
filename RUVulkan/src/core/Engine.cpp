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

	LoadGameObjects();

	return true;
}

void Engine::Run()
{
	SimpleRenderSystem renderSystem{ device, renderer.GetSwapChainRenderPass() };
	Camera camera{};
	//camera.SetViewDirection(glm::vec3{ 0 }, glm::vec3{ .5f, 0.f, 1.f });
	camera.SetViewTarget(glm::vec3{ -1.f, -2.f, 2.f }, glm::vec3{ 0.f, 0.f, 2.5f });

	auto viewGameObject = GameObject::CreateGameObject();

	auto currentTime = std::chrono::high_resolution_clock::now();

	while (run)
	{
		while (SDL_PollEvent(&e))
		{
			if (e.type == SDL_QUIT)
				run = false;
			if (e.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				OnResize();
				std::cout << "Window Resized" << std::endl;
			}
		}
		Input::Listen();
		auto newTime = std::chrono::high_resolution_clock::now();

		float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
		currentTime = newTime;

		MoveInPlaneZ(frameTime, viewGameObject);

		camera.SetViewYXZ(viewGameObject.transform.translation, viewGameObject.transform.rotation);

		float aspect = renderer.GetSwapChainAspectRatio();
		camera.SetPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 100.f);
		//camera.SetOrthoProjection(-aspect, aspect, -1, 1, -1, 1);

		if (auto commandBuffer = renderer.BeginFrame())
		{
			renderer.BeginSwapChainRenderPass(commandBuffer);
			renderSystem.RenderGameObjects(commandBuffer, gameObjects, camera);
			renderer.EndSwapChainRenderPass(commandBuffer);
			renderer.EndFrame();
		}
	}

	vkDeviceWaitIdle(device.device());
}

void Engine::Shutdown()
{
	

	SDL_Quit();
}

void Engine::LoadGameObjects()
{
	std::shared_ptr<Model> flatVase = Model::CreateModelFromFile(device, "res/Models/flat_vase.obj");
	
	auto gameObject1 = GameObject::CreateGameObject();
	gameObject1.model = flatVase;
	gameObject1.transform.translation = { 0.5f, 0.5f, 2.5f };
	gameObject1.transform.scale = glm::vec3{ 3.f, 1.5f, 3.f };
	gameObjects.push_back(std::move(gameObject1));

	std::shared_ptr<Model> smoothVase = Model::CreateModelFromFile(device, "res/Models/smooth_vase.obj");

	auto gameObject = GameObject::CreateGameObject();
	gameObject.model = smoothVase;
	gameObject.transform.translation = { -.5f, 0.5f, 2.5f };
	gameObject.transform.scale = glm::vec3{ 3.f, 1.5f, 3.f };
	gameObjects.push_back(std::move(gameObject));
}


void Engine::OnResize()
{
	renderer.OnResize();
}

