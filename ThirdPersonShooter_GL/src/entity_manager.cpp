#include "entity_manager.hpp"
#include "physics_world.hpp"

static std::vector<std::unique_ptr<Entity>> entities;
static int current_light;
static int light_count;

void EntityManager::init()
{

}

GLuint EntityManager::get_current_light()
{
	return current_light;
}

void EntityManager::change_light()
{
	current_light++;
	if (current_light == light_count)
	{
		current_light = 0;
	}
}

GLuint EntityManager::get_num_lights()
{
	return light_count;
}

Entity* EntityManager::get_entity(GLuint index)
{
	return entities[index].get();
}

Entity* EntityManager::get_last()
{
	return entities[entities.size() - 1].get();
}

Entity* EntityManager::get_first()
{
	return entities[0].get();
}

void EntityManager::add_entity(std::unique_ptr<Entity> entity)
{
	if (entity == nullptr)
	{
		std::clog << "EntityManager::add_entity: entity is null" << std::endl;
		return;
	}
	
	if (entity->get_component<Light>())
	{
		light_count++;
	}

	entity->set_id(entities.size());
	entities.push_back(std::move(entity));
}

void EntityManager::remove_entity(Entity* entity)
{ 
	for (GLuint i = 0; i < entities.size(); i++)
	{
		if (entity == entities[i].get())
		{
			PhysicsBody* pb;
			if (pb = entities[i]->get_component<PhysicsBody>())
			{
				PhysicsWorld::remove_physics_body(pb);
			}
			entities.erase(std::remove(entities.begin(), entities.end(), entities[i]), entities.end());
			return;
		}
	}
}

void EntityManager::update_entities()
{
	ControllerComponent* cc;
	for (GLuint e = 0; e < entities.size(); e++)
	{
		for (GLuint c = 0; c < entities[e]->get_components().size(); c++)
		{
			if (cc = dynamic_cast<ControllerComponent*>(entities[e]->get_component(c)))
			{
				((ControllerComponent*)entities[e]->get_component(c))->update();
			}
		}
	}
}

void EntityManager::draw_entities(GLuint material_index)
{
	DrawableComponent* dc = nullptr;
	for (GLuint e = 0; e < entities.size(); e++)
	{
		if (dc = entities[e]->get_component<DrawableComponent>())
		{
			if (dc->material == material_index && dc->visible == 1)
			{
				dc->draw();
			}
		}
	}
}
