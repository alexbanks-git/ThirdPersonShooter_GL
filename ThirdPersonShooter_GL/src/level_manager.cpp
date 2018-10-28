#include "level_manager.hpp"
#include "graphics.hpp"
#include "entity_manager.hpp"
#include "physics_world.hpp"
#include "skybox.hpp"

static Skybox current_skybox;
static bool paused;

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

void LevelManager::pause_level(bool pause)
{
	paused = pause;
}

bool LevelManager::is_paused()
{
	return paused;
}

void LevelManager::update_level()
{
	EntityManager::update_ui();
	if (paused)
	{
		return;
	}
	EntityManager::update_entities();
	PhysicsWorld::update_physics_world(Graphics::get_elapsed_time(), 10);
}

void LevelManager::draw_level()
{
	EntityManager::draw_entities(0);
}

void LevelManager::draw_lights()
{
	GLint light_count_location = glGetUniformLocation(Shader::get_shader("default_shader"), "light_count");
	glUniform1i(light_count_location, EntityManager::get_num_lights());
	EntityManager::draw_lights();
}

void LevelManager::draw_ui()
{
	EntityManager::draw_ui();
}

void LevelManager::draw_skybox()
{
	current_skybox.draw_skybox();
}

void LevelManager::draw_images()
{
	EntityManager::draw_entities(2);
}

void LevelManager::create_shadows()
{
	EntityManager::create_shadows();
}