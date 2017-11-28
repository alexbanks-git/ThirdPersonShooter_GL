#include "entity_manager.hpp"

static std::vector<Entity*> entities;

void EntityManager::init()
{

}

void EntityManager::add_entity(Entity* entity)
{
	entities.push_back(entity);
}

void EntityManager::update_entities()
{

}

void EntityManager::draw_entities()
{
	for (GLuint e = 0; e < entities.size(); e++)
	{
		for (GLuint c = 0; c < entities[e]->get_components().size(); c++)
		{
			if (dynamic_cast<DrawableComponent*>(entities[e]->get_component(c)) != nullptr)
			{
				((DrawableComponent*)entities[e]->get_component(c))->draw();
			}
		}
	}
}
