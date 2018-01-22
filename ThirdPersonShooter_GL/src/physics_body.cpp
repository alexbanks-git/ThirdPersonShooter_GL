#include <iostream>
#include "physics_body.hpp"
#include "physics_world.hpp"
#include "entity.hpp"

PhysicsBody::PhysicsBody(Entity* entity) : Component(entity)
{

}

void PhysicsBody::create(std::string shape, GLfloat mass, glm::vec3 size, glm::vec3 offset)
{
	center_offset = offset;
	if (shape == "Capsule")
	{
		collision_shape = std::make_unique<btCapsuleShape>(btCapsuleShape(size.x, size.y));
	}
	else if (shape == "Box")
	{
		collision_shape = std::make_unique<btBoxShape>(btBoxShape(btVector3(size.z, size.y, size.x)));
	}
	else if (shape == "StaticPlane")
	{
		collision_shape = std::make_unique<btStaticPlaneShape>(btStaticPlaneShape(btVector3(0.0f, 1.0f, 0.0f), transform.position.y));
	}
	else if (shape == "Sphere")
	{
		collision_shape = std::make_unique<btSphereShape>(btSphereShape(size.x));
	}
	width = size.x;
	height = size.y;
	btVector3 inertia(0.0f, 0.0f, 0.0f);
	motion_state = btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1),
		btVector3(transform.position.x, transform.position.y, transform.position.z)),
		btTransform(btQuaternion(0, 0, 0, 1),
			btVector3(0, 0, 0)));
	collision_shape->calculateLocalInertia(mass, inertia);
	btRigidBody::btRigidBodyConstructionInfo construction_info(mass, &motion_state, collision_shape.get(), inertia);
	rigid_body = std::make_unique<btRigidBody>(btRigidBody(construction_info));
	rigid_body->setUserPointer(&owner);
	kinematic = false;
	PhysicsWorld::add_physics_body(this);
}

void PhysicsBody::update()
{
	btTransform trans = rigid_body->getCenterOfMassTransform();
	btVector3 center = trans.getOrigin();

	if (!kinematic)
		transform.set_rotation(to_glm_mat(trans.getBasis()));
	else
		rigid_body->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f));

	glm::vec3 delta = glm::vec3();
	delta.y = center.y() + center_offset.y;
	delta.x = center.x() + center_offset.z;
	delta.z = center.z() + center_offset.x;
	transform.set_position(delta);
}

std::string PhysicsBody::type_name()
{
	return "PhysicsBody";
}

void PhysicsBody::set_mass(GLfloat m)
{
}

void PhysicsBody::set_inertia(glm::vec3 i)
{
}

btRigidBody* PhysicsBody::get_rigid_body()
{
	return rigid_body.get();
}

void PhysicsBody::apply_impulse(glm::vec3 force)
{
	rigid_body->activate();
	rigid_body->applyCentralImpulse(btVector3(force.x, force.y, force.z));
}

bool PhysicsBody::is_kinematic()
{
	return kinematic;
}

void PhysicsBody::set_kinematic(bool value)
{
	kinematic = value;
}

glm::mat3 PhysicsBody::to_glm_mat(btMatrix3x3& matrix)
{
	btMatrix3x3 new_matrix = matrix.transpose();
	glm::mat3 glm_mat = glm::mat3();
	glm_mat[0][0] = new_matrix[0][0];
	glm_mat[0][1] = new_matrix[0][1];
	glm_mat[0][2] = new_matrix[0][2];

	glm_mat[1][0] = new_matrix[1][0];
	glm_mat[1][1] = new_matrix[1][1];
	glm_mat[1][2] = new_matrix[1][2];

	glm_mat[2][0] = new_matrix[2][0];
	glm_mat[2][1] = new_matrix[2][1];
	glm_mat[2][2] = new_matrix[2][2];
	return glm_mat;
}

void PhysicsBody::set_velocity(glm::vec3 vel)
{
	rigid_body->activate();
	rigid_body->setLinearVelocity(btVector3(vel.x, vel.y, vel.z));
}

glm::vec3 PhysicsBody::linear_velocity()
{
	btVector3 vel = rigid_body->getLinearVelocity();
	return glm::vec3(vel.getX(), vel.getY(), vel.getZ());
}

void PhysicsBody::move(glm::vec3 velocity)
{
	rigid_body->activate();
	btTransform trans;
	trans.setOrigin(rigid_body->getWorldTransform().getOrigin() + btVector3(velocity.x, velocity.y, velocity.z));
	trans.setRotation(rigid_body->getWorldTransform().getRotation());
	rigid_body->setWorldTransform(trans);
	rigid_body->getMotionState()->setWorldTransform(trans);
}