#pragma once
#include "physics_body.hpp"
#include "physics_debug_drawer.hpp"

class PhysicsWorld
{
public:
	/**
	 * @brief Initializes the physics world.
	 */
	static void init_physics();

	/**
	 * @brief Draws all objects in the physics world.
	 */
	static void draw_physics_world();

	/**
	 * @brief Advances the physics world by the specified time step
	 * @param time_step the time step to advance the physics world by
	 * @param max_sub_steps
	 */
	static void update_physics_world(GLfloat time_step, GLfloat max_sub_steps);

	/**
	 * @brief Sets the gravity of the physics world.
	 * @param g the value to set the gravity to
	 */
	static void set_gravity(glm::vec3 g);

	/**
	 * @brief Adds a physics body to the physics world.
	 * @param body the physics body to add to the world
	 */
	static void add_physics_body(PhysicsBody* body);
};