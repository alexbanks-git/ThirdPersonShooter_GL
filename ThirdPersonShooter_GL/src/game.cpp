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
	Spawn::spawn_runner(glm::vec3(0.0f, 1.0f, 0.0f), &player);

	/*Spawn::spawn_weapon(glm::vec3(0.0f, 0.0f, 0.0f), "survivor.fbx", &player,
		&player.get_component<Model>()->skeleton.get_bone_at(35)->final_transform);*/

	Spawn::spawn_image(glm::vec3(800 / 2, 600 / 2, 0.0f), "crosshair.png");
	Spawn::spawn_plane(0.0f);
	GLint mouse_x;
	GLint mouse_y;
	GLfloat pitch = 0;
	GLfloat yaw = 0;
	Uint32 mouseState;
	const Uint8* keystate = SDL_GetKeyboardState(NULL);

	glm::vec3 light_pos = glm::vec3(0, 50, 0);
	glm::mat4 light_mat = glm::translate(glm::mat4(), light_pos);
	//SDL_SetRelativeMouseMode(SDL_TRUE);
	while (running)
	{
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

		LevelManager::update_level();

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		//glDisable(GL_CULL_FACE);
		glUseProgram(Graphics::get_skybox_shader());
		GLint view_uniform = glGetUniformLocation(Graphics::get_skybox_shader(), "view");
		glUniformMatrix4fv(view_uniform, 1, GL_FALSE, glm::value_ptr(glm::mat4(glm::mat3(view_matrix))));

		GLint projection_uniform = glGetUniformLocation(Graphics::get_skybox_shader(), "projection");
		glUniformMatrix4fv(projection_uniform, 1, GL_FALSE, glm::value_ptr(projection_matrix));

		LevelManager::draw_skybox();
		//glDepthFunc(GL_LESS);

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glUseProgram(Graphics::get_default_shader());
		GLint camera_uniform = glGetUniformLocation(Graphics::get_default_shader(), "camera_position");
		glUniform3f(camera_uniform, camera->transform.position.x, camera->transform.position.y,
			camera->transform.position.z);

		GLint light_mat_uniform = glGetUniformLocation(Graphics::get_default_shader(), "light_mat");
		glUniformMatrix4fv(light_mat_uniform, 1, GL_FALSE, glm::value_ptr(light_mat));

		GLint light_uniform = glGetUniformLocation(Graphics::get_default_shader(), "light");
		glUniform3f(light_uniform, light_pos.x, light_pos.y, light_pos.z);

		view_uniform = glGetUniformLocation(Graphics::get_default_shader(), "view");
		glUniformMatrix4fv(view_uniform, 1, GL_FALSE, glm::value_ptr(view_matrix));

		projection_uniform = glGetUniformLocation(Graphics::get_default_shader(), "projection");
		glUniformMatrix4fv(projection_uniform, 1, GL_FALSE, glm::value_ptr(projection_matrix));

		LevelManager::draw_level();

		glDisable(GL_CULL_FACE);
		glUseProgram(Graphics::get_image_shader());

		view_uniform = glGetUniformLocation(Graphics::get_image_shader(), "view");
		glUniformMatrix4fv(view_uniform, 1, GL_FALSE, glm::value_ptr(view_matrix));

		projection_uniform = glGetUniformLocation(Graphics::get_image_shader(), "projection");
		glUniformMatrix4fv(projection_uniform, 1, GL_FALSE, glm::value_ptr(projection_matrix));

		LevelManager::draw_images();

		glUseProgram(Graphics::get_debug_shader());

		view_uniform = glGetUniformLocation(Graphics::get_debug_shader(), "view");
		glUniformMatrix4fv(view_uniform, 1, GL_FALSE, glm::value_ptr(view_matrix));

		projection_uniform = glGetUniformLocation(Graphics::get_debug_shader(), "projection");
		glUniformMatrix4fv(projection_uniform, 1, GL_FALSE, glm::value_ptr(projection_matrix));

		//PhysicsWorld::draw_physics_world();

		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glUseProgram(Graphics::get_user_interface_shader());
		view_uniform = glGetUniformLocation(Graphics::get_user_interface_shader(), "view");
		view_matrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(view_uniform, 1, GL_FALSE, glm::value_ptr(view_matrix));

		projection_uniform = glGetUniformLocation(Graphics::get_user_interface_shader(), "projection");
		projection_matrix = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.0f, 200.0f);
		glUniformMatrix4fv(projection_uniform, 1, GL_FALSE, glm::value_ptr(projection_matrix));
		LevelManager::draw_ui();

		Graphics::update_window();
		//SDL_WarpMouseInWindow(Graphics::get_window(), width / 2, height / 2);
	}
	return 0;
}