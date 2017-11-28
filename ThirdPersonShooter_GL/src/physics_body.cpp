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
	height = size.y;
	this->mass = mass;
	inertia = btVector3(0.0f, 0.0f, 0.0f);
	motion_state = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), 
	btVector3(transform->position.x, transform->position.y+height/2, transform->position.z)));
	collision_shape->calculateLocalInertia(mass, inertia);
	btRigidBody::btRigidBodyConstructionInfo construction_info(mass, motion_state, collision_shape, inertia);
	rigid_body = new btRigidBody(construction_info);
	PhysicsWorld::add_physics_body(this);
}

void PhysicsBody::update()
{
	btTransform trans;
	rigid_body->getMotionState()->getWorldTransform(trans);

	transform->translate(glm::vec3(trans.getOrigin().getX(), trans.getOrigin().getY() - height / 2, 
		trans.getOrigin().getZ()));
	rigid_body->activate();
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

void PhysicsBody::move(glm::vec3 velocity)
{
	btVector3 impulse = btVector3(0.0f, velocity.y, 0.0f);
	btVector3 vel = btVector3(velocity.x, rigid_body->getLinearVelocity().getY(), velocity.z);
	rigid_body->setLinearVelocity(vel);
	rigid_body->applyCentralImpulse(impulse);
	rigid_body->setAngularFactor(btVector3(0.0f, 0.0f, 0.0f));
}