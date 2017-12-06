
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

	Camera camera = Camera(glm::vec3(0.0f, 100.0f, -300.0f), glm::radians(60.0f), 1.0f, 1000.0f);

	GLuint width = 800;
	GLuint height = 600;

	Graphics::init_graphics(width, height, 60);
	GLboolean running = true;
	PhysicsWorld::init_physics();
	PhysicsWorld::set_gravity(glm::vec3(0.0f, -200.0f, 0.0f));
	LevelManager::init_level();

	Entity entity = Entity();
	Entity ground = Entity();
	/*Entity entity_2 = Entity();
	Entity entity_3 = Entity();*/

	ground.transform.translate(glm::vec3(0.0f, -20.0f, 0.0f));
	entity.transform.translate(glm::vec3(0.0f, 100.0f, 0.0f));/*
	entity_2.transform.translate(glm::vec3(70.0f, 50.0f, 20.0f));
	entity_3.transform.translate(glm::vec3(120.0f, 50.0f, -40.0f));

	entity_2.add_component("Model");
	entity_3.add_component("Model");

	((Model*)entity_2.get_component("Model"))->create("parasite_l_starkie.fbx");
	((Model*)entity_3.get_component("Model"))->create("parasite_l_starkie.fbx");*/

	entity.add_component("AnimationController");

	entity.add_component("Model");
	std::vector<std::string> anim_paths;
	anim_paths.push_back("survivor_idle.fbx");
	anim_paths.push_back("survivor_run.fbx");
	anim_paths.push_back("survivor_jump.fbx");
	((Model*)entity.get_component("Model"))->create(
		"survivor.fbx", anim_paths, ((AnimationController*)entity.get_component("AnimationController")));

	entity.add_component("PhysicsBody");
	Model model = *((Model*)entity.get_component("Model"));
	((PhysicsBody*)entity.get_component("PhysicsBody"))->create(
		"Capsule", 1.0f, glm::vec3(model.get_max_bounds().z, model.get_max_bounds().y, model.get_max_bounds().x));

	ground.add_component("PhysicsBody");
	((PhysicsBody*)ground.get_component("PhysicsBody"))->create(
		"StaticPlane", 0.0f, glm::vec3(100.0f, 1.0f, 100.0f));

	entity.add_component("PlayerController");
	((PlayerController*)entity.get_component("PlayerController"))->set_camera(&camera.transform);

	EntityManager::add_entity(&entity);/*
	EntityManager::add_entity(&entity_2);
	EntityManager::add_entity(&entity_3);*/

	GLint mouse_x;
	GLint mouse_y;
	GLfloat pitch = 0;
	GLfloat yaw = 0;
	Uint32 mouseState = SDL_GetMouseState(&mouse_x, &mouse_y);
	const Uint8* keystate = SDL_GetKeyboardState(NULL);

	glm::vec3 light_pos = glm::vec3(50, 50, -50);
	glm::mat4 light_mat = glm::translate(glm::mat4(), light_pos);
	SDL_SetRelativeMouseMode(SDL_TRUE);
	while (running)
	{
		SDL_PumpEvents();
		mouseState = SDL_GetRelativeMouseState(&mouse_x, &mouse_y);

		if (keystate[SDL_SCANCODE_ESCAPE])
			running = false;

		glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		

		Transform camera_target = entity.transform;
		camera_target.position.y += 125;

		if (mouse_x > 20)
			yaw = 20.0f;
		else if (mouse_x < -20)
			yaw = -20.0f;
		else
			yaw = 0;

		if (mouse_y > 20)
			pitch = 20.0f;
		else if (mouse_y < -20)
			pitch = -20.0f;
		else
			pitch = 0;

		camera.follow(camera_target, -300);
		camera.rotate_around(camera_target, yaw, pitch);

		/*if (keystate[SDL_SCANCODE_W])
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
		}*/

		glm::mat4 projection_matrix;
		projection_matrix = glm::perspective(
			camera.get_field_of_view(),
			(GLfloat)(width / height),
			camera.near_clip_plane(),
			camera.far_clip_plane());

		glm::mat4 view_matrix;

		view_matrix = glm::lookAt(
			camera.transform.position,
			camera.transform.position + camera.transform.forward,
			Transform::world_up_vector());
		((PlayerController*)entity.get_component("PlayerController"))->update();
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
		glUniform3f(camera_uniform, camera.transform.position.x, camera.transform.position.y,
			camera.transform.position.z);

		GLint light_mat_uniform = glGetUniformLocation(Graphics::get_default_shader(), "light_mat");
		glUniformMatrix4fv(light_mat_uniform, 1, GL_FALSE, glm::value_ptr(light_mat));

		GLint light_uniform = glGetUniformLocation(Graphics::get_default_shader(), "light");
		glUniform3f(light_uniform, light_pos.x, light_pos.y, light_pos.z);

		view_uniform = glGetUniformLocation(Graphics::get_default_shader(), "view");
		glUniformMatrix4fv(view_uniform, 1, GL_FALSE, glm::value_ptr(view_matrix));

		projection_uniform = glGetUniformLocation(Graphics::get_default_shader(), "projection");
		glUniformMatrix4fv(projection_uniform, 1, GL_FALSE, glm::value_ptr(projection_matrix));

		LevelManager::draw_level();

		glUseProgram(Graphics::get_debug_shader());

		view_uniform = glGetUniformLocation(Graphics::get_debug_shader(), "view");
		glUniformMatrix3fv(view_uniform, 1, GL_FALSE, glm::value_ptr(view_matrix));

		projection_uniform = glGetUniformLocation(Graphics::get_debug_shader(), "projection");
		glUniformMatrix4fv(projection_uniform, 1, GL_FALSE, glm::value_ptr(projection_matrix));

		PhysicsWorld::draw_physics_world();
		Graphics::update_window();
		SDL_WarpMouseInWindow(Graphics::get_window(), width / 2, height / 2);
	}
	return 0;
}