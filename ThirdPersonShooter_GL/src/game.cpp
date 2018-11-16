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
#include "spawn.hpp"
#include "input_manager.hpp"

void pause(Button* button)
{
		LevelManager::pause_level(!LevelManager::is_paused());
}

int main(int argc, char* args[])
{
	//DrawClass debugDraw = DrawClass();
	//debugDraw.setDebugMode(btIDebugDraw::DBG_DrawWireframe);

	GLuint width = 800;
	GLuint height = 600;

	Graphics::init_graphics(width, height, 60.0f);
	GLuint result;

	GLboolean running = true;
	PhysicsWorld::init_physics();
	PhysicsWorld::set_gravity(glm::vec3(0.0f, -10.0f, 0.0f));
	LevelManager::init_level();
	Entity& cam_entity = Spawn::spawn_camera(
		glm::vec3(0.0f, 2.0f, -6.0f), glm::radians(60.0f), 1.0f, 10000.0f);

	Camera* camera = cam_entity.get_component<Camera>();
	Entity& player = Spawn::spawn_player(glm::vec3(0.0f, 1.0f, -4.0f), camera);
	cam_entity.get_component<CameraController>()->set_target(&player);

	Entity& light = Spawn::spawn_directional_light(glm::normalize(glm::vec3(0.0f, -1.0f, 1.0f)), 0.7f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	
	//Spawn::spawn_cube(glm::vec3(0.0f, 0.0f, 1.0f), true);
	Spawn::spawn_model(glm::vec3(-3.0f, 1.2f, -4.0f), "wall.fbx");
	Spawn::spawn_model(glm::vec3(-2.0f, 1.2f, 0.0f), "wall.fbx");
	Spawn::spawn_model(glm::vec3(1.0f, 1.2f, 2.0f), "wall.fbx");
	Spawn::spawn_model(glm::vec3(7.0f, 1.2f, 6.0f), "wall.fbx");
	Spawn::spawn_model(glm::vec3(-5.0f, 1.2f, 7.0f), "wall.fbx");

	Spawn::spawn_cube(glm::vec3(0.0f, 4.0f, -2.0f), false);
	Spawn::spawn_cube(glm::vec3(2.0f, 4.0f, -2.0f), false);
	Spawn::spawn_cube(glm::vec3(-2.0f, 4.0f, -2.0f), false);
	Spawn::spawn_cube(glm::vec3(-2.0f, 4.0f, -1.0f), false);

	Spawn::spawn_image(glm::vec3(800 / 2, 600 / 2, 0.0f), 3, 3, "crosshair.png");
	Spawn::spawn_plane(0.0f);
	GLint mouse_x;
	GLint mouse_y;
	GLfloat pitch = 0;
	GLfloat yaw = 0;
	Uint32 mouseState;
	const Uint8* keystate = SDL_GetKeyboardState(NULL);
	//SDL_SetRelativeMouseMode(SDL_TRUE);

	while (running)
	{
		InputManager::init();
		SDL_PumpEvents();

		if (keystate[SDL_SCANCODE_ESCAPE])
			running = false;

		Graphics::clear_screen(0.5f, 0.5f, 0.5f, 1.0f);

		glm::mat4 projection_matrix;
		projection_matrix = glm::perspective(
			camera->get_field_of_view(),
			(GLfloat)(width / height),
			camera->near_clip_plane(),
			camera->far_clip_plane());

		glm::mat4 view_matrix;

		view_matrix = glm::lookAt(
			camera->transform.position,
			camera->transform.position + camera->transform.forward,
			Transform::world_up_vector());

		Graphics::set_matrices(projection_matrix, view_matrix);

		LevelManager::update_level();

		if (keystate[SDL_SCANCODE_RCTRL])
		{
			
		}

		glDepthFunc(GL_LEQUAL);
		//glDisable(GL_CULL_FACE);
		Shader::use_shader("skybox_shader");
		GLint view_uniform = glGetUniformLocation(Shader::get_current_shader(), "view");
		glUniformMatrix4fv(view_uniform, 1, GL_FALSE, glm::value_ptr(glm::mat4(glm::mat3(view_matrix))));

		GLint projection_uniform = glGetUniformLocation(Shader::get_current_shader(), "projection");
		glUniformMatrix4fv(projection_uniform, 1, GL_FALSE, glm::value_ptr(projection_matrix));

		LevelManager::draw_skybox();
		//glDepthFunc(GL_LESS);

		Shader::use_shader("shadow_shader");
		LevelManager::create_shadows();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		Shader::use_shader("default_shader");
		glViewport(0, 0, Graphics::get_window_width(), Graphics::get_window_height());
		GLint camera_uniform = glGetUniformLocation(Shader::get_current_shader(), "camera_position");
		glUniform3f(camera_uniform, camera->transform.position.x, camera->transform.position.y,
			camera->transform.position.z);

		view_uniform = glGetUniformLocation(Shader::get_current_shader(), "view");
		glUniformMatrix4fv(view_uniform, 1, GL_FALSE, glm::value_ptr(view_matrix));

		projection_uniform = glGetUniformLocation(Shader::get_current_shader(), "projection");
		glUniformMatrix4fv(projection_uniform, 1, GL_FALSE, glm::value_ptr(projection_matrix));
		
		LevelManager::draw_lights();

		LevelManager::draw_level();

		
		Shader::use_shader("image_shader");
		view_uniform = glGetUniformLocation(Shader::get_current_shader(), "view");
		glUniformMatrix4fv(view_uniform, 1, GL_FALSE, glm::value_ptr(view_matrix));

		projection_uniform = glGetUniformLocation(Shader::get_current_shader(), "projection");
		glUniformMatrix4fv(projection_uniform, 1, GL_FALSE, glm::value_ptr(projection_matrix));

		LevelManager::draw_images();

		/* Shader::use_shader("physics_shader");

		view_uniform = glGetUniformLocation(Shader::get_current_shader(), "view");
		glUniformMatrix4fv(view_uniform, 1, GL_FALSE, glm::value_ptr(view_matrix));

		projection_uniform = glGetUniformLocation(Shader::get_current_shader(), "projection");
		glUniformMatrix4fv(projection_uniform, 1, GL_FALSE, glm::value_ptr(projection_matrix));

		PhysicsWorld::draw_physics_world();*/

		Shader::use_shader("user_interface_shader");
		view_uniform = glGetUniformLocation(Shader::get_current_shader(), "view");
		view_matrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(view_uniform, 1, GL_FALSE, glm::value_ptr(view_matrix));

		projection_uniform = glGetUniformLocation(Shader::get_current_shader(), "projection");
		projection_matrix = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, 0.0f, 200.0f);
		glUniformMatrix4fv(projection_uniform, 1, GL_FALSE, glm::value_ptr(projection_matrix));
		LevelManager::draw_ui();

		Graphics::update_window();
		//SDL_WarpMouseInWindow(Graphics::get_window(), width / 2, height / 2);
	}
	return 0;
}