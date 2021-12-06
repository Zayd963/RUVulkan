#pragma once

#include "Model.h"
#include <memory>
#include <gtc/matrix_transform.hpp>
#include <unordered_map>
class GameObject
{
public:
	using id_t = unsigned int;
	using Map = std::unordered_map<id_t, GameObject>;
	static GameObject CreateGameObject()
	{
		static id_t currentID;
		return GameObject(currentID++);
	}

	GameObject(const GameObject&) = delete;
	GameObject& operator = (const GameObject&) = delete;
	GameObject(GameObject&&) = default;
	GameObject& operator = (GameObject&&)=default;
	id_t GetID() { return ID; }
	
	std::shared_ptr<Model> model;
	glm::vec4 color{};

	struct TransformComponent
	{
		glm::vec3 translation;
		glm::vec3 scale = { 1.0f, 1.0f , 1.0f};
		glm::vec3 rotation;

		glm::mat4 Mat4();
		glm::mat3 NormalMatrix();

	};
	TransformComponent transform{};
private:

	GameObject(id_t id) : ID(id) {};

	id_t ID;

};

