#include "drawable_component.hpp"

DrawableComponent::DrawableComponent(Entity* entity) : Component(entity)
{
	visible = 1;
}

void DrawableComponent::draw()
{
	
}

std::string DrawableComponent::type_name()
{
	return "DrawableComponent";
}