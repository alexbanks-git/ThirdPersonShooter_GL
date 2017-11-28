#include "component.hpp"
#include "entity.hpp"

void Component::set_owner(Entity* entity)
{
	owner = entity;
	transform = &entity->transform;
}

std::string Component::type_name()
{
	return "Component";
}