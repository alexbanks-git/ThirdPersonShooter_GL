#pragma once
#include <memory>
#include "entity.hpp"

class EntityManager
{
public:
	
	static GLuint get_num_lights();
	static void change_light();
	static GLuint get_current_light();
	static void init();

	/**
	 * @brief Adds an entity to the game.
	 * @param entity the entity to add
	 */
	static void add_entity(std::unique_ptr<Entity> entity);

	/**
	 * @brief Returns a pointer to the entity at the specified index.
	 * @param index the index of the entity
	 * @returns a pointer to the entity
	 */
	static Entity* get_entity(GLuint index);

	/**
	 * @brief Returns a pointer to the last entity added to the game.
	 * @returns pointer to the entity
	 */
	static Entity* get_last();

	static Entity* get_last_light();

	static Entity* get_last_ui_element();

	static Entity* get_first_ui_element();

	static void add_ui_element(std::unique_ptr<Entity> entity);

	/**
	 * @brief Returns a pointer to the first entity added to the game.
	 * @returns pointer to the entity
	 */
	static Entity* get_first();

	static Entity* get_first_light();

	/**
	 * @brief Removes an entity from the game.
	 * @param entity the entity to remove
	 */
	static void EntityManager::remove_entity(Entity* entity);

	/**
	 * @brief Updates controller components of all entities.
	 */
	static void update_entities();

	/**
	 * @brief Draws drawable components of all entities with the specified material index.
	 * @param material_index the material of the entities that should be drawn
	 */
	static void draw_entities(GLuint material_index);

	static void create_shadows();

	static void draw_lights();

	static void draw_ui();

	static void update_ui();
};