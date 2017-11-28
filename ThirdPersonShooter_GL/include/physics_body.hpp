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
	GLfloat height;
	btRigidBody* rigid_body;
public:
	
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

	void update();

	btRigidBody* get_rigid_body();

	void move(glm::vec3 velocity);
};