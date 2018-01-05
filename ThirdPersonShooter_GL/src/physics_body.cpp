#include <iostream>
#include "physics_body.hpp"
#include "physics_world.hpp"

void PhysicsBody::create(std::string shape, GLfloat mass, glm::vec3 size)
{

	if (shape == "Capsule")
	{
		collision_shape = new btCapsuleShape(size.x, size.y);
	}
	else if (shape == "Box")
	{
		collision_shape = new btBoxShape(btVector3(size.z, size.y/2, size.x));
	}
	else if (shape == "StaticPlane")
	{
		collision_shape = new btStaticPlaneShape(btVector3(0.0f, 1.0f, 0.0f), -20.0f);
	}
	localVelocity = glm::vec3();
	width = size.x;
	height = size.y;
	this->mass = mass;
	inertia = btVector3(0.0f, 0.0f, 0.0f);
	motion_state = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), 
	btVector3(transform->position.x, transform->position.y+height/2, transform->position.z)));
	collision_shape->calculateLocalInertia(mass, inertia);
	btRigidBody::btRigidBodyConstructionInfo construction_info(mass, motion_state, collision_shape, inertia);
	rigid_body = new btRigidBody(construction_info);
	PhysicsWorld::add_physics_body(this);
/*
	btTransform trans;
	rigid_body->getMotionState()->getWorldTransform(trans);

	transform->translate(glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY() - height / 2,
		trans.getOrigin().getZ()));*/
}

void PhysicsBody::update()
{
	btTransform trans;
	rigid_body->getMotionState()->getWorldTransform(trans);

	GLfloat delta_y = (trans.getOrigin().getY() - height / 2) - transform->position.y;
	GLfloat delta_x = trans.getOrigin().getX() - transform->position.x;
	GLfloat delta_z = trans.getOrigin().getZ() - transform->position.z;
	transform->global_translate(glm::vec3(delta_x, delta_y, delta_z));
	
}

std::string PhysicsBody::type_name()
{
	return "PhysicsBody";
}

void PhysicsBody::set_mass(GLfloat m)
{
	mass = m;
}

void PhysicsBody::set_inertia(glm::vec3 i)
{
	inertia.setValue(i.x, i.y, i.z);
}

btRigidBody* PhysicsBody::get_rigid_body()
{
	return rigid_body;
}

void PhysicsBody::apply_impulse(glm::vec3 force)
{
	rigid_body->activate();
	rigid_body->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f));
	rigid_body->applyCentralImpulse(btVector3(force.x, force.y, force.z));
}

void PhysicsBody::set_velocity(glm::vec3 vel)
{
	rigid_body->activate();
	rigid_body->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f));
	rigid_body->setLinearVelocity(btVector3(vel.x, vel.y, vel.z));
}

glm::vec3 PhysicsBody::linear_velocity()
{
	btVector3 vel = rigid_body->getLinearVelocity();
	return glm::vec3(vel.getX(), vel.getY(), vel.getZ());
}

glm::vec3 PhysicsBody::local_linear_velocity()
{
	return localVelocity;
}

void PhysicsBody::move(glm::vec3 velocity)
{
	rigid_body->activate();
	rigid_body->translate(btVector3(velocity.x, velocity.y, velocity.z));
}