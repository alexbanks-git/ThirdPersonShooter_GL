#include "entity_manager.hpp"
#include "physics_world.hpp"

static std::vector<std::unique_ptr<Entity>> entities;
static std::vector<std::unique_ptr<Entity>> lights;
static std::vector<std::unique_ptr<Entity>> ui_elements;
static int current_light;
static int light_count;

void EntityManager::init()
{

}

Entity* EntityManager::get_last_ui_element()
{
	return ui_elements[ui_elements.size() - 1].get();
}

Entity* EntityManager::get_first_ui_element()
{
	return ui_elements[0].get();
}

void EntityManager::add_ui_element(std::unique_ptr<Entity> entity)
{
	if (entity == nullptr)
	{
		std::clog << "EntityManager::add_ui_element: entity is null" << std::endl;
		return;
	}

	entity->set_id(ui_elements.size());
	ui_elements.push_back(std::move(entity));
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

void EntityManager::create_shadows()
{
	for (GLuint i = 0; i < lights.size(); i++)
	{
		if (lights[i]->get_component<Light>()->is_directional_light())
		{
			lights[i]->get_component<Light>()->create_shadows();
			break;
		}
	}
}

Entity* EntityManager::get_first_light()
{
	return lights[0].get();
}

Entity* EntityManager::get_last_light()
{
	return lights[lights.size() - 1].get();
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
		if (entity->get_component<Light>()->is_point_light())
		{
			light_count++;
		}

		entity->set_id(lights.size());
		lights.push_back(std::move(entity));
	}
	else
	{
		entity->set_id(entities.size());
		entities.push_back(std::move(entity));
	}
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

void EntityManager::update_ui()
{
	ControllerComponent* cc;
	for (GLuint e = 0; e < ui_elements.size(); e++)
	{
		for (GLuint c = 0; c < ui_elements[e]->get_components().size(); c++)
		{
			if (cc = dynamic_cast<ControllerComponent*>(ui_elements[e]->get_component(c)))
			{
				((ControllerComponent*)ui_elements[e]->get_component(c))->update();
			}
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
	for (GLuint i = 0; i < lights.size(); i++)
	{
		if (lights[i]->get_component<Light>()->is_directional_light())
		{
			lights[i]->get_component<Light>()->apply_depth_map();
			break;
		}
	}
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

void EntityManager::draw_lights()
{
	for (GLuint i = 0; i < lights.size(); i++)
	{
		lights[i]->get_component<DrawableComponent>()->draw();
	}
}

void EntityManager::draw_ui()
{
	DrawableComponent* dc = nullptr;
	for (GLuint i = 0; i < ui_elements.size(); i++)
	{
		if (dc = ui_elements[i]->get_component<DrawableComponent>())
		{
			dc->draw();
		}
	}
}
