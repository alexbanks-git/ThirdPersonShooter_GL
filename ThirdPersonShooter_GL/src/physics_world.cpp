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

	solver = new btSequentialImpulseConstraintSolver;

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
}
void PhysicsWorld::set_gravity(glm::vec3 g)
{
	btVector3 gravity;
	gravity.setValue(g.x, g.y, g.z);
	dynamics_world->setGravity(gravity);
}
void PhysicsWorld::add_physics_body(PhysicsBody* body)
{
	dynamics_world->addRigidBody(body->get_rigid_body());
	physics_bodies.push_back(body);
}