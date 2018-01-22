#include <memory>
#include <algorithm>
#include "physics_world.hpp"

static btDynamicsWorld* dynamics_world;
static btBroadphaseInterface* broad_phase;
static PhysicsDebugDrawer* debug_draw;
static btDefaultCollisionConfiguration* collision_config;
static btCollisionDispatcher* dispatcher;
static btSequentialImpulseConstraintSolver* solver;
static std::vector<PhysicsBody*> physics_bodies;

void PhysicsWorld::init_physics()
{
	broad_phase = new btDbvtBroadphase();
	debug_draw = new PhysicsDebugDrawer();

	collision_config = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collision_config);

	solver = new btSequentialImpulseConstraintSolver();

	dynamics_world = new btDiscreteDynamicsWorld(dispatcher, broad_phase, solver, collision_config);
	debug_draw->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	dynamics_world->setDebugDrawer(debug_draw);
}

void PhysicsWorld::draw_physics_world()
{
	debug_draw->clearLines();
	dynamics_world->debugDrawWorld();
}
void PhysicsWorld::update_physics_world(GLfloat time_step, GLfloat max_sub_steps)
{
	dynamics_world->stepSimulation(time_step, max_sub_steps);
	for (GLuint i = 0; i < physics_bodies.size(); i++)
	{
		physics_bodies[i]->update();
	}
}

void PhysicsWorld::remove_physics_body(PhysicsBody* body)
{
	dynamics_world->removeRigidBody(body->get_rigid_body());
	physics_bodies.erase(std::remove(physics_bodies.begin(), physics_bodies.end(), body), physics_bodies.end());
}

void PhysicsWorld::set_gravity(glm::vec3 g)
{
	btVector3 gravity;
	gravity.setValue(g.x, g.y, g.z);
	dynamics_world->setGravity(gravity);
}
void PhysicsWorld::add_physics_body(PhysicsBody* body)
{
	if (body == nullptr)
	{
		std::clog << "PhysicsWorld::add_physics_body: body is null" << std::endl;
		return;
	}
	dynamics_world->addRigidBody(body->get_rigid_body());
	physics_bodies.push_back(body);
} 

PhysicsWorld::RayHit* PhysicsWorld::ray_cast(glm::vec3 start, glm::vec3 direction, GLfloat distance)
{
	glm::vec3 end_vector = start + direction * distance;
	btVector3 bt_start = btVector3(start.x, start.y, start.z);
	btVector3 bt_end = btVector3(end_vector.x, end_vector.y, end_vector.z);
	btCollisionWorld::ClosestRayResultCallback ray_callback(bt_start, bt_end);
	dynamics_world->rayTest(bt_start, bt_end, ray_callback);

	if (ray_callback.hasHit())
	{
		void* point = ray_callback.m_collisionObject->getUserPointer();
		if (point != nullptr)
		{
			RayHit* hit = new RayHit();
			hit->entity = (Entity*)point;
			hit->hit_point = glm::vec3(ray_callback.m_hitPointWorld.x(), ray_callback.m_hitPointWorld.y(), ray_callback.m_hitPointWorld.z());
			return hit;
		}
	}

	return nullptr;
}

bool PhysicsWorld::on_ground(PhysicsBody* body)
{
	glm::vec3 raycast_origin = body->transform.get_position();
	raycast_origin.y -= (body->height/2 + body->center_offset.y);
	return ray_cast(raycast_origin, -Transform::world_up_vector(), 0.4f) != nullptr;
}