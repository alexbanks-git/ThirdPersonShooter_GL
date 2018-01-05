#pragma once
#include "GL/glew.h"
#include <btBulletDynamicsCommon.h>
#include "component.hpp"
#include "transform.hpp"

class PhysicsBody : public Component
{

private:
	
	btCollisionShape* collision_shape;
	btScalar mass;
	btVector3 inertia;
	btDefaultMotionState* motion_state;
	btRigidBody* rigid_body;
	glm::vec3 localVelocity;
public:
	GLfloat height;
	GLfloat width;
	/**
	 * @brief Creates a rigid body with a specified shape and size.
	 * @param shape the shape of the rigid body
	 * @param width the width of the rigid body
	 * @param height the height of the rigid body
	 * @param depth the depth of the rigid body
	 */
	void PhysicsBody::create(std::string shape, GLfloat mass, glm::vec3 size);

	/**
	 * @brief Sets the mass of the rigid body.
	 * @param m the mass to be used
	 */
	void set_mass(GLfloat m);

	/**
	 * @brief Sets the inertia of the rigid body.
	 * @param i the inertia to be used
	 */
	void set_inertia(glm::vec3 i);

	/**
	 * @brief Retrieves the type of the class.
	 * @returns the class type
	 */
	std::string type_name();

	/**
	 * @brief Keeps the physics body's transform updated.
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

	void set_velocity(glm::vec3 vel);

	glm::vec3 linear_velocity();

	glm::vec3 local_linear_velocity();
};