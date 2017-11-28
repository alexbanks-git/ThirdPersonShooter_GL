#include "level_manager.hpp"
#include "graphics.hpp"
#include "entity_manager.hpp"
#include "physics_world.hpp"

void LevelManager::init_level()
{
}

void LevelManager::update_level()
{
	PhysicsWorld::update_physics_world(1/30.0f, 10.0f);
}

void LevelManager::draw_level()
{
	EntityManager::draw_entities();
}