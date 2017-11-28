#include "entity.hpp"
#include "model.hpp"
#include "physics_body.hpp"
#include "player_controller.hpp"

void Entity::add_component(std::string type)
{
	Component* component = new Component();

	if (type == "Model")
	{
		component = new Model();
	}
	else if (type == "PhysicsBody")
	{
		component = new PhysicsBody();
	}
	else if (type == "PlayerController")
	{
		component = new PlayerController();
	}

	component->set_owner(this);
	components.push_back(component);
}

Component* Entity::get_component(std::string type)
{
	for (GLuint i = 0; i < components.size(); i++)
	{
		if (components[i]->type_name() == type)
		{
			return components[i];
		}
	}
	return nullptr;
}

Component* Entity::get_component(GLuint i)
{
	return components[i];
}

std::vector<Component*> Entity::get_components()
{
	return components;
}