#pragma once
#include "physics_body.hpp"
#include "physics_debug_drawer.hpp"

class PhysicsWorld
{
public:
	class RayHit
	{
	public:
		Entity* entity;
		glm::vec3 hit_point;
	};
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

	/**
	 * @brief Removes a physics body from the world.
	 * @param body the physics body to remove from the world
	 */
	static void remove_physics_body(PhysicsBody* body);

	/**
	 * @brief Performs a raycast from a specified starting position for a certain distance.
	 * @param start the starting position of the raycast
	 * @param direction the direction of the raycast
	 * @param distance how far the raycast extends
	 * @returns the object that was hit as a RayHit object
	 */
	static RayHit* ray_cast(glm::vec3 start, glm::vec3 direction, GLfloat distance);

	/**
	 * @brief Returns whether or not the specified physics body is on the ground.
	 * @param body a pointer to the physics body
	 */
	static bool on_ground(PhysicsBody* body);
};