#include "entity.hpp"
#include "model.hpp"
#include "physics_body.hpp"
#include "player_controller.hpp"
#include "animation_controller.hpp"

Entity::Entity()
{
	transform.set_position(glm::vec3(0.0f, 0.0f, 0.0f));
	transform.look_at(transform.position + glm::vec3(0.0f, 0.0f, 1.0f));
}

Component* Entity::get_component(GLuint i)
{
	return components[i].get();
}

std::vector<Component*> Entity::get_components()
{
	std::vector<Component*> new_vector = std::vector<Component*>();
	for (GLuint i = 0; i < components.size(); i++)
	{
		new_vector.push_back(components[i].get());
	}
	return new_vector;
}

GLuint Entity::get_id()
{
	return id;
}

void Entity::set_id(GLuint index)
{
	id = index;
}