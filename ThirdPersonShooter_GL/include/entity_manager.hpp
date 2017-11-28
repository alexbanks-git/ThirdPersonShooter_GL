#pragma once
#include "entity.hpp"

class EntityManager
{
public:
	static void init();

	/**
	 * @brief Adds an entity to the game.
	 * @param entity the entity to add
	 */
	static void add_entity(Entity* entity);

	/**
	 * @brief Updates controller components of all entities.
	 */
	static void update_entities();

	/**
	 * @brief Draws drawable components of all entities.
	 */
	static void draw_entities();
};