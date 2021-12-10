#pragma once

#include <vector>
#include "../core/Pipeline/MasterRenderer.h"
class Scene
{
public:
	Scene(EngineDevice &dev, Window& window, Renderer& _renderer);
	virtual void Update(float frameTime);
	virtual void Render();
	Camera& GetCamera() { return sceneCamera; }
private:
	void LoadGameObjects(EngineDevice& device);
	
	EngineDevice& device;
	Camera sceneCamera{};
	
	Renderer& renderer;
	GameObject::Map gameObjects; 
	std::unique_ptr<MasterRenderer> masterRenderer;
};

