#include "controller_component.hpp"

ControllerComponent::ControllerComponent(Entity* entity) : Component(entity)
{

}

void ControllerComponent::update()
{
}

std::string ControllerComponent::type_name()
{
	return "ControllerComponent";
}