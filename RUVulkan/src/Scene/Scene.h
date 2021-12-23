#pragma once

#include <vector>
#include "../core/Pipeline/MasterRenderer.h"
#include "../core/Systems/CameraController.h"
class Scene
{
public:
	Scene(EngineDevice &dev, Window& window, Renderer& _renderer);
	virtual void Update(float frameTime);
	virtual void Render();
	Camera& GetCamera() { return sceneCamera; }
private:
	void LoadGameObjects(EngineDevice& device);
	
	glm::vec3 lastMouseVelocity{};
	glm::vec3 currentMouseVelocity{};

	EngineDevice& device;
	Camera sceneCamera{};
	GlobalUBO ubo{};
	Renderer& renderer;
	GameObject::Map gameObjects; 
	std::unique_ptr<MasterRenderer> masterRenderer;
	GameObject viewGameObject = GameObject::CreateGameObject();
	CameraController camera{ viewGameObject };

};

