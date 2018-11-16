#include "GL/glew.h"
#include "SDL.h"
#include "player_controller.hpp"
#include "entity.hpp"
#include "entity_manager.hpp"
#include "physics_body.hpp"
#include "physics_world.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "spawn.hpp"
#include "input_manager.hpp"

PlayerController::PlayerController(Entity* entity) : ControllerComponent(entity)
{
	jump_force = 1.5f;
	run_speed = 5.0f;
}

void PlayerController::init_game_controller()
{
	game_controller = SDL_GameControllerOpen(0);

}

void PlayerController::update()
{
	a_button = InputManager::button_pressed(SDL_CONTROLLER_BUTTON_A);
	b_button = InputManager::button_pressed(SDL_CONTROLLER_BUTTON_B);
	left_trigger = InputManager::get_axis(SDL_CONTROLLER_AXIS_TRIGGERLEFT);
	right_trigger = InputManager::get_axis(SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
	left_stick_x = InputManager::get_axis(SDL_CONTROLLER_AXIS_LEFTX);
	left_stick_y = InputManager::get_axis(SDL_CONTROLLER_AXIS_LEFTY);

	GLint mouse_x = 0;
	GLint mouse_y = 0;
	const Uint8* keystate = SDL_GetKeyboardState(NULL);
	Uint32 mouse = SDL_GetMouseState(&mouse_x, &mouse_y);
	PhysicsBody* body = owner.get_component<PhysicsBody>();
	GLfloat forward_speed = 0.0f;
	glm::vec3 direction = transform.forward;
	glm::vec3 new_dir;
	GLfloat bones_angle = 0.0f;
	glm::vec3 move_dir = glm::vec3();


	if (current_action != Action::Cover)
	{
		current_action = Action::None;
	}

	if (keystate[SDL_SCANCODE_C] || b_button)
	{
		if (current_action == Action::Cover && cover_timer >= 30.0f)
		{
			current_action = Action::None;
			transform.look_at(transform.position - transform.forward);
			cover_timer = 0.0f;
		}
		else
		{
			PhysicsWorld::RayHit* hit = PhysicsWorld::ray_cast(transform.position, transform.forward, 1.0f);
			if (hit != NULL)
			{
				cover = hit->entity;
				current_action = Action::Cover;
				play_animation(PlayerAnimation::Cover_Idle, true);
				glm::vec3 lookDir = (transform.position - hit->hit_point);
				lookDir.y = 0.0f;
				lookDir = glm::normalize(lookDir);
				transform.look_at(transform.position + lookDir);

				cover_left = glm::cross(lookDir, Transform::world_up_vector());
				cover_right = -cover_left;
				cover_move_dir = cover_right;
			}
			else if (current_action != Action::Cover)
			{
				play_animation(PlayerAnimation::Roll);
			}
		}
	}

	if (current_action == Action::Cover)
	{
		camera->get_owner().get_component<CameraController>()->set_zoom_availability(false);
		cover_timer++;
		if (keystate[SDL_SCANCODE_A])
		{
			PhysicsWorld::RayHit* hit = PhysicsWorld::ray_cast(transform.position - transform.right *
				owner.get_component<PhysicsBody>()->width / 2.0f, -transform.forward, 5.0f);
			if (hit == NULL || hit->entity != cover)
			{
				cover_speed = 0.0f;
			}
			else
			{
				cover_speed = 0.5f;
				cover_move_dir = cover_left;
				play_animation(PlayerAnimation::Cover_Right, true);
			}

		}
		else if (keystate[SDL_SCANCODE_D])
		{
			PhysicsWorld::RayHit* hit = PhysicsWorld::ray_cast(transform.position + transform.right *
				owner.get_component<PhysicsBody>()->width / 2.0f, -transform.forward, 5.0f);
			if (hit == NULL || hit->entity != cover)
			{
				cover_speed = 0.0f;
			}
			else
			{
				cover_speed = 0.5f;
				cover_move_dir = cover_right;
				play_animation(PlayerAnimation::Cover_left, true);
			}
		}
		else
		{
			cover_speed = 0.0f;
			if (cover_move_dir == cover_right)
			{
				play_animation(PlayerAnimation::Cover_Idle_Opposite, true);
			}
			else
			{
				play_animation(PlayerAnimation::Cover_Idle, true);
			}
		}

		glm::vec3 forward_velocity = glm::vec3();
		forward_velocity = cover_move_dir * cover_speed;
		body->set_velocity(glm::vec3(forward_velocity.x, body->linear_velocity().y, forward_velocity.z));
		return;
	}

	if ((new_dir = change_facing_direction(keystate)) != glm::vec3())
	{
		forward_speed = run_speed;
		direction = glm::normalize(new_dir);
	}

	if (owner.get_component<AnimationController>()->get_active_animation() == 5)
	{
		if (owner.get_component<AnimationController>()->animation_playing())
		{
			current_action = Action::Rolling;
		}
	}

	if (PhysicsWorld::on_ground(body) && current_action != Action::Rolling)// && current_action != Action::Jumping)
	{
		if ((mouse & SDL_BUTTON(SDL_BUTTON_LEFT)) || right_trigger > 0)
		{
			bones_angle = camera->transform.forward.y;
			current_action = Action::Shooting;
			move_dir = direction;
			direction = camera->transform.forward;

			if (!firing_bullet)
			{
				firing_bullet = true;
			}
		}
		else if (mouse & SDL_BUTTON(SDL_BUTTON_RIGHT) || left_trigger > 0)
		{
			move_dir = direction;
			direction = camera->transform.forward;
			bones_angle = camera->transform.forward.y;
			current_action = Action::Aiming;
		}

		if (!(mouse & SDL_BUTTON(SDL_BUTTON_LEFT)))
		{
			firing_bullet = false;
		}

		rotate_bones(bones_angle);

		if (forward_speed == 0.0f && body->linear_velocity().y >= -0.01f && body->linear_velocity().y <= 0.01f)
		{
			owner.get_component<AnimationController>()->play_animation(current_animation(), true);
		}
		else
		{
			if (current_action != Action::Aiming && current_action != Action::Shooting)
			{
				play_animation(PlayerAnimation::Run, true);
			}
		}

		if (keystate[SDL_SCANCODE_SPACE] || a_button)
		{
			jump();
		}

		glm::vec3 forward_velocity = glm::vec3();
		direction.y = 0.0f;
		transform.look_at(transform.position + direction);

		if (current_action != Action::Aiming && current_action != Action::Shooting)
		{
			forward_velocity = direction * forward_speed;
		}

		body->set_velocity(glm::vec3(forward_velocity.x, body->linear_velocity().y, forward_velocity.z));
	}
	if (SDL_GetTicks() - shoot_start_time >= 200 && firing_bullet)
	{
		shoot_start_time = SDL_GetTicks();
		fire_bullet();
	}

	camera->get_owner().get_component<CameraController>()->set_zoom_availability(current_action == Action::Aiming || current_action == Action::Shooting);
}

void PlayerController::set_camera(Camera* cam)
{
	if (cam == nullptr)
	{
		std::clog << "PlayerController::set_camera: cam is null" << std::endl;
		return;
	}
	camera = cam;
	camera->get_owner().get_component<CameraController>()->set_vertical_offset(2.0f);
}

std::string PlayerController::type_name()
{
	return "PlayerController";
}

void PlayerController::rotate_bones(GLfloat angle)
{
	owner.get_component<Model>()->skeleton.rotate_bone(6, angle, glm::vec3(-1, 0, 0));
	owner.get_component<Model>()->skeleton.rotate_bone(8, angle, glm::vec3(-1, 0, 0));
	owner.get_component<Model>()->skeleton.rotate_bone(32, angle, glm::vec3(-1, 0, 0));
}

GLuint PlayerController::current_animation()
{
	GLuint index = 0;
	if (current_action == Action::Aiming)
	{
		index = 3;
	}
	else if (current_action == Action::Shooting)
	{
		index = 4;
	}

	return index;
}

void PlayerController::jump()
{
	current_action = Action::Jumping;
	owner.get_component<AnimationController>()->play_animation(2);
}

void PlayerController::fire_bullet()
{
	PhysicsWorld::RayHit* hit = PhysicsWorld::ray_cast(
		camera->transform.position, camera->transform.forward, 3000.0f);

	glm::vec3 bullet_velocity = glm::vec3();
	glm::vec3 bullet_end = glm::vec3();
	glm::vec3 bullet_pos = glm::vec3(transform.position.x, transform.position.y + 0.5, transform.position.z) +
		camera->transform.forward;

	if (hit != nullptr)
	{
		bullet_end = hit->hit_point;
		if (hit->entity->get_component<MonsterController>())
		{
			EntityManager::remove_entity(hit->entity);
		}
		else if (hit->entity->get_component<PhysicsBody>())
		{
			hit->entity->get_component<PhysicsBody>()->apply_impulse(
				glm::normalize(bullet_end - bullet_pos) * 5.0f);
		}
	}
	else
	{
		bullet_end = camera->transform.position + camera->transform.forward * 3000.0f;
	}



	glm::vec3 vec = glm::normalize(bullet_end - bullet_pos);

	bullet_velocity = vec;

	Entity& bullet = Spawn::spawn_bullet(bullet_pos, bullet_velocity, bullet_end, "crosshair.png");
	//bullet.transform.look_at(transform.position + transform.right);
	//bullet.transform.right_look_at(bullet_end);
}

glm::vec3 PlayerController::change_facing_direction(const Uint8* keys)
{
	glm::vec3 direction = glm::vec3();
	if (keys[SDL_SCANCODE_W])
	{
		direction = camera->transform.forward;
	}
	else if (keys[SDL_SCANCODE_S])
	{
		direction = -camera->transform.forward;
	}

	if (keys[SDL_SCANCODE_A])
	{
		direction += camera->transform.right;
	}
	else if (keys[SDL_SCANCODE_D])
	{
		direction += -camera->transform.right;
	}
	//if (left_stick_x > 0 && left_stick_x < 18000)
	//{
	//	left_stick_x = 0.0f;
	//}

	//if (left_stick_x < 0 && left_stick_x > -18000)
	//{
	//	left_stick_x = 0.0f;
	//}
	//
	//if (left_stick_y > 0 && left_stick_y < 18000)
	//{
	//	left_stick_y = 0.0f;
	//}

	//if (left_stick_y < 0 && left_stick_y > -18000)
	//{
	//	left_stick_y = 0.0f;
	//}
	//
	//direction = glm::vec3(-left_stick_x, 0.0f, -left_stick_y);
	//direction.y = 0;
	return direction;
}

void PlayerController::play_animation(PlayerAnimation anim, bool loop)
{
	owner.get_component<AnimationController>()->play_animation((GLuint)anim, loop);
}