#include "component.hpp"
#include "entity.hpp"

Component::Component(Entity* entity) : owner(*entity), transform(owner.transform)
{

}

Entity& Component::get_owner()
{
	return owner;
}

std::string Component::type_name()
{
	return "Component";
}