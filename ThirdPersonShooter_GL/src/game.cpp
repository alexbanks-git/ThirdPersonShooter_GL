
#include "camera.hpp"
#include "asset_loader.hpp"
#include "entity.hpp"
#include "graphics.hpp"
#include "model.hpp"
#include "physics_body.hpp"
#include "physics_world.hpp"
#include "physics_debug_drawer.hpp"
#include "entity_manager.hpp"
#include "level_manager.hpp"
#include "player_controller.hpp"


int main(int argc, char* args[])
{
	//DrawClass debugDraw = DrawClass();
	//debugDraw.setDebugMode(btIDebugDraw::DBG_DrawWireframe);

	Camera camera = Camera(glm::vec3(0.0f, 0.0f, -300.0f), glm::radians(60.0f), 1.0f, 1000.0f);
	camera.rotate(0.0f, 90.0f, 0.0f);

	GLuint width = 800;
	GLuint height = 600;

	Graphics::init_graphics(width, height, 60);
	GLboolean running = true;
	PhysicsWorld::init_physics();
	PhysicsWorld::set_gravity(glm::vec3(0.0f, -50.0f, 0.0f));

	Entity entity = Entity();
	Entity ground = Entity();

	ground.transform.translate(glm::vec3(0.0f, -20.0f, 0.0f));
	entity.transform.translate(glm::vec3(0.0f, 100.0f, 0.0f));

	entity.add_component("Model");
	((Model*)entity.get_component("Model"))->create("Knight.fbx");

	entity.add_component("PhysicsBody");
	Model model = *((Model*)entity.get_component("Model"));
	((PhysicsBody*)entity.get_component("PhysicsBody"))->create(
		"Capsule", 1.0f, glm::vec3(model.get_max_bounds().z, model.get_max_bounds().y, model.get_max_bounds().x));

	ground.add_component("PhysicsBody");
	((PhysicsBody*)ground.get_component("PhysicsBody"))->create(
		"StaticPlane", 0.0f, glm::vec3(100.0f, 1.0f, 100.0f));

	entity.add_component("PlayerController");
	

	EntityManager::add_entity(&entity);

	GLint mouseX;
	GLint mouseY;
	Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);
	const Uint8* keystate = SDL_GetKeyboardState(NULL);

	glm::vec3 light_pos = glm::vec3(50, 50, -50);
	glm::mat4 light_mat = glm::translate(glm::mat4(), light_pos);

	while (running)
	{
		SDL_PumpEvents();
		mouseState = SDL_GetMouseState(&mouseX, &mouseY);

		if (keystate[SDL_SCANCODE_ESCAPE])
			running = false;

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glUseProgram(Graphics::get_default_shader());

		camera.rotate(mouseY, mouseX, 0.0f);

		if (keystate[SDL_SCANCODE_W])
		{
			camera.translate(0.0f, 0.0f, 1.0f);
		}
		else if (keystate[SDL_SCANCODE_S])
		{
			camera.translate(0.0f, 0.0f, -1.0f);
		}

		if (keystate[SDL_SCANCODE_A])
		{
			camera.translate(1.0f, 0.0f, 0.0f);
		}
		else if (keystate[SDL_SCANCODE_D])
		{
			camera.translate(-1.0f, 0.0f, 0.0f);
		}

		glm::mat4 projection_matrix;
		projection_matrix = glm::perspective(
			camera.get_field_of_view(),
			(GLfloat)(width / height),
			camera.near_clip_plane(),
			camera.far_clip_plane());

		glm::mat4 view_matrix;

		view_matrix = glm::lookAt(
			camera.position,
			camera.position + camera.forward,
			glm::vec3(0.0f, 1.0f, 0.0f));
		((PlayerController*)entity.get_component("PlayerController"))->update();
		LevelManager::update_level();

		GLint camera_uniform = glGetUniformLocation(Graphics::get_default_shader(), "camera_position");
		glUniform3f(camera_uniform, camera.position.x, camera.position.y,
			camera.position.z);

		GLint light_mat_uniform = glGetUniformLocation(Graphics::get_default_shader(), "light_mat");
		glUniformMatrix4fv(light_mat_uniform, 1, GL_FALSE, glm::value_ptr(light_mat));

		GLint light_uniform = glGetUniformLocation(Graphics::get_default_shader(), "light");
		glUniform3f(light_uniform, light_pos.x, light_pos.y, light_pos.z);

		GLint view_uniform = glGetUniformLocation(Graphics::get_default_shader(), "view");
		glUniformMatrix4fv(view_uniform, 1, GL_FALSE, glm::value_ptr(view_matrix));

		GLint projection_uniform = glGetUniformLocation(Graphics::get_default_shader(), "projection");
		glUniformMatrix4fv(projection_uniform, 1, GL_FALSE, glm::value_ptr(projection_matrix));

		LevelManager::draw_level();

		glUseProgram(Graphics::get_debug_shader());

		view_uniform = glGetUniformLocation(Graphics::get_debug_shader(), "view");
		glUniformMatrix4fv(view_uniform, 1, GL_FALSE, glm::value_ptr(view_matrix));

		projection_uniform = glGetUniformLocation(Graphics::get_debug_shader(), "projection");
		glUniformMatrix4fv(projection_uniform, 1, GL_FALSE, glm::value_ptr(projection_matrix));

		PhysicsWorld::draw_physics_world();

		Graphics::update_window();
	}
	return 0;
}