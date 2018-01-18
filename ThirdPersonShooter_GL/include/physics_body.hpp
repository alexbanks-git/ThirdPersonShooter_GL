#pragma once
#include <memory>
#include "GL/glew.h"
#include <btBulletDynamicsCommon.h>
#include "component.hpp"
#include "transform.hpp"

class PhysicsBody : public Component
{

private:
	std::unique_ptr<btCollisionShape> collision_shape;
	btDefaultMotionState motion_state;
	std::unique_ptr<btRigidBody> rigid_body;
	bool kinematic;
public:
	PhysicsBody(Entity* entity);
	GLfloat height;
	GLfloat width;
	/**
	 * @brief Creates a physics body with a specified shape and size.
	 * @param shape the shape of the physics body
	 * @param size the width, height, and depth of the physics body
	 */
	void PhysicsBody::create(std::string shape, GLfloat mass, glm::vec3 size);

	/**
	 * @brief Sets the mass of the physics body.
	 * @param m the mass to be used
	 */
	void set_mass(GLfloat m);

	/**
	 * @brief Sets the inertia of the physics body.
	 * @param i the inertia to be used
	 */
	void set_inertia(glm::vec3 i);

	/**
	 * @brief Retrieves the type of the class.
	 * @returns the class type
	 */
	std::string type_name();

	/**
	 * @brief Updates the physics body's transform.
	 */
	void update();

	/**
	 * @brief Retrieves the rigid body of the physics body.
	 * @returns the rigid body attached to this physics body
	 */
	btRigidBody* get_rigid_body();

	/**
	 * @brief Moves the physics body in the world.
	 * @param velocity the direction and speed to move the physics body
	 */
	void move(glm::vec3 velocity);

	/**
	 * @brief Applies an instant force to the physics body.
	 * @param force the amount and direction of the force
	 */
	void apply_impulse(glm::vec3 force);

	/**
	 * @brief Sets the velocity of the physics body.
	 * @param vel the value to set the physics body's velocity to
	 */
	void set_velocity(glm::vec3 vel);

	/**
	 * @brief Returns the linear velocity of the physics body.
	 * @returns the linear velocity 
	 */
	glm::vec3 linear_velocity();

	/**
	 * @brief Returns whether or not the physics body is kinematic.
	 * @returns true if the physics body is kinematic, and false if not
	 */
	bool is_kinematic();

	/**
	 * @brief Sets whether or not the physics body is kinematic
	 * @param value 
	 */
	void set_kinematic(bool value);

	glm::mat3 to_glm_mat(btMatrix3x3& matrix);
};