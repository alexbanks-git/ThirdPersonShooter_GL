#include "component.hpp"
#include "entity.hpp"

void Component::set_owner(Entity* entity)
{
	if (entity == nullptr)
	{
		std::clog << "Component::set_owner: entity is null" << std::endl;
		return;
	}
	owner = entity;
	transform = &entity->transform;
}

std::string Component::type_name()
{
	return "Component";
}