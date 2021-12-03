#include "Engine.h"
#include <iostream>
#include <array>
#include "Systems/SimpleRenderSystem.h"
#define GLM_FORCE_RADIANS
#define FORCE_DEPTH_ZERO_TO_ONE
#include <glm.hpp>
#include <gtc/constants.hpp>
#include "Pipeline/Camera.h"
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

std::unique_ptr<Model> createCubeModel(EngineDevice& device, glm::vec3 offset) {
	std::vector<Model::Vertex> vertices = {};
	vertices = {
		// left face (white)
		{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f, 1.0f}},
		{{-.5f, .5f, .5f}, {.9f, .9f, .9f, 1.0f}},
		{{-.5f, -.5f, .5f}, {.9f, .9f, .9f, 1.0f}},
		{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f, 1.0f}},
		{{-.5f, .5f, -.5f}, {.9f, .9f, .9f, 1.0f}},
		{{-.5f, .5f, .5f}, {.9f, .9f, .9f, 1.0f}},

		// right face (yellow)
		{{.5f, -.5f, -.5f}, {.8f, .8f, .1f, 1.0f}},
		{{.5f, .5f, .5f}, {.8f, .8f, .1f, 1.0f}},
		{{.5f, -.5f, .5f}, {.8f, .8f, .1f, 1.0f}},
		{{.5f, -.5f, -.5f}, {.8f, .8f, .1f, 1.0f}},
		{{.5f, .5f, -.5f}, {.8f, .8f, .1f, 1.0f}},
		{{.5f, .5f, .5f}, {.8f, .8f, .1f, 1.0f}},

		// top face (orange, remember y axis points down)
		{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f, 1.0f}},
		{{.5f, -.5f, .5f}, {.9f, .6f, .1f, 1.0f}},
		{{-.5f, -.5f, .5f}, {.9f, .6f, .1f, 1.0f}},
		{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f, 1.0f}},
		{{.5f, -.5f, -.5f}, {.9f, .6f, .1f, 1.0f}},
		{{.5f, -.5f, .5f}, {.9f, .6f, .1f, 1.0f}},

		// bottom face (red)
		{{-.5f, .5f, -.5f}, {.8f, .1f, .1f, 1.0f}},
		{{.5f, .5f, .5f}, {.8f, .1f, .1f, 1.0f}},
		{{-.5f, .5f, .5f}, {.8f, .1f, .1f, 1.0f}},
		{{-.5f, .5f, -.5f}, {.8f, .1f, .1f, 1.0f}},
		{{.5f, .5f, -.5f}, {.8f, .1f, .1f, 1.0f}},
		{{.5f, .5f, .5f}, {.8f, .1f, .1f, 1.0f}},

		// nose face (blue)
		{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f, 1.0f}},
		{{.5f, .5f, 0.5f}, {.1f, .1f, .8f, 1.0f}},
		{{-.5f, .5f, 0.5f}, {.1f, .1f, .8f, 1.0f}},
		{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f, 1.0f}},
		{{.5f, -.5f, 0.5f}, {.1f, .1f, .8f, 1.0f}},
		{{.5f, .5f, 0.5f}, {.1f, .1f, .8f, 1.0f}},

		// tail face (green)
		{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f, 1.0f}},
		{{.5f, .5f, -0.5f}, {.1f, .8f, .1f, 1.0f}},
		{{-.5f, .5f, -0.5f}, {.1f, .8f, .1f, 1.0f}},
		{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f, 1.0f}},
		{{.5f, -.5f, -0.5f}, {.1f, .8f, .1f, 1.0f}},
		{{.5f, .5f, -0.5f}, {.1f, .8f, .1f, 1.0f}},

	};
	for (auto& v : vertices) {
		v.position += offset;
	}
	return std::make_unique<Model>(device, vertices);
}

void Engine::LoadGameObjects()
{
	std::shared_ptr<Model> model = createCubeModel(device, glm::vec3(0));
	
	auto cube = GameObject::CreateGameObject();
	cube.model = model;
	cube.transform.translation = { 0.f, 0.f, 2.5f };
	cube.transform.scale = { 0.5f, 0.5f , 0.5f };
	gameObjects.push_back(std::move(cube));
}


void Engine::OnResize()
{
	renderer.OnResize();
}

