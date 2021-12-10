#include "Scene.h"
#include <memory>
#include <iostream>

Scene::Scene(EngineDevice& dev, Window& window, Renderer& _renderer)
	:device(dev), renderer(_renderer)
{
	masterRenderer = std::make_unique<MasterRenderer>(device, renderer);
	float aspect = renderer.GetSwapChainAspectRatio();
	sceneCamera.SetPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 1000.f);
	sceneCamera.SetViewTarget(glm::vec3{ -1.f, -2.f, 2.f }, glm::vec3{ 0.f, 0.f, 2.5f });
	LoadGameObjects(dev);
}

float light = 0;
float increment = 1.f;
void Scene::Update(float frameTime)
{
	if (light > 2)
		increment = -1;
	else if (light < -2)
		increment = 1;

	light += increment * frameTime;
	//ubo.lightDirection.x = light;
	
}

void Scene::Render()
{
	float aspect = renderer.GetSwapChainAspectRatio();
	sceneCamera.SetPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 1000.f);
	masterRenderer->Draw(sceneCamera, gameObjects);
}

void Scene::LoadGameObjects(EngineDevice& device)
{

	std::shared_ptr<Model> flatVase = Model::CreateModelFromFile(device, "res/Models/flat_vase.obj");

	auto gameObject1 = GameObject::CreateGameObject();
	gameObject1.model = flatVase;
	gameObject1.transform.translation = { 0.f, 0.f, 0.f };
	gameObject1.transform.scale = glm::vec3{ 3.f, 1.5f, 3.f };
	gameObjects.emplace(gameObject1.GetID(), std::move(gameObject1));

	std::shared_ptr<Model> smoothVase = Model::CreateModelFromFile(device, "res/Models/smooth_vase.obj");

	auto gameObject = GameObject::CreateGameObject();
	gameObject.model = smoothVase;
	gameObject.transform.translation = { -1.f, 0.f, 0.f };
	gameObject.transform.scale = glm::vec3{ 3.f, 1.5f, 3.f };
	gameObjects.emplace(gameObject.GetID(), std::move(gameObject));

	std::shared_ptr<Model> Quad = Model::CreateModelFromFile(device, "res/Models/Quad.obj");

	auto floor = GameObject::CreateGameObject();
	floor.model = Quad;
	floor.transform.translation = { 0.f, 0.f, 0.f };
	floor.transform.scale = glm::vec3{ 3.f, 1.f, 3.f };
	gameObjects.emplace(floor.GetID(), std::move(floor));
	

	std::shared_ptr colouredCube = Model::CreateModelFromFile(device, "res/Models/colored_cube.obj");

	auto cube = GameObject::CreateGameObject();
	cube.model = colouredCube;
	cube.transform.translation = { -1.f, -.5f, .5f };
	cube.transform.scale = glm::vec3{ .5f, .5f, .5f };
	gameObjects.emplace(cube.GetID(), std::move(cube));

}
