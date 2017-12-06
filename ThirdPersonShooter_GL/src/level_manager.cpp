#include "level_manager.hpp"
#include "graphics.hpp"
#include "entity_manager.hpp"
#include "physics_world.hpp"
#include "skybox.hpp"

static Skybox current_skybox;

void LevelManager::init_level()
{
	current_skybox = Skybox();

	std::vector<std::string> faces;

	faces.push_back("C:/Users/sharg_000/Documents/ThirdPersonShooter_GL/ThirdPersonShooter_GL/ThirdPersonShooter_GL/skybox/right.jpg");
	faces.push_back("C:/Users/sharg_000/Documents/ThirdPersonShooter_GL/ThirdPersonShooter_GL/ThirdPersonShooter_GL/skybox/left.jpg");
	faces.push_back("C:/Users/sharg_000/Documents/ThirdPersonShooter_GL/ThirdPersonShooter_GL/ThirdPersonShooter_GL/skybox/top.jpg");
	faces.push_back("C:/Users/sharg_000/Documents/ThirdPersonShooter_GL/ThirdPersonShooter_GL/ThirdPersonShooter_GL/skybox/bottom.jpg");
	faces.push_back("C:/Users/sharg_000/Documents/ThirdPersonShooter_GL/ThirdPersonShooter_GL/ThirdPersonShooter_GL/skybox/back.jpg");
	faces.push_back("C:/Users/sharg_000/Documents/ThirdPersonShooter_GL/ThirdPersonShooter_GL/ThirdPersonShooter_GL/skybox/front.jpg");
	current_skybox.generate_skybox(faces);
}

void LevelManager::update_level()
{
	EntityManager::update_entities();
	PhysicsWorld::update_physics_world(1/30.0f, 10.0f);
}

void LevelManager::draw_level()
{
	EntityManager::draw_entities();
}

void LevelManager::draw_skybox()
{
	current_skybox.draw_skybox();
}