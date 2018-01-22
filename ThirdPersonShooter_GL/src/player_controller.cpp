#include "GL/glew.h"
#include "SDL.h"
#include "player_controller.hpp"
#include "entity.hpp"
#include "entity_manager.hpp"
#include "physics_body.hpp"
#include "physics_world.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "spawn.hpp"

PlayerController::PlayerController(Entity* entity) : ControllerComponent(entity)
{
	jump_force = 1.5f;
	run_speed = 5.0f;
}

void PlayerController::update()
{
	GLint mouse_x = 0;
	GLint mouse_y = 0;
	const Uint8* keystate = SDL_GetKeyboardState(NULL);
	Uint32 mouse = SDL_GetMouseState(&mouse_x, &mouse_y);
	PhysicsBody* body = owner.get_component<PhysicsBody>();
	GLfloat forward_speed = 0.0f;
	glm::vec3 direction = transform.forward;
	glm::vec3 new_dir;
	GLfloat bones_angle = 0.0f;

	if ((new_dir = change_facing_direction(keystate)) != glm::vec3())
	{
		forward_speed = run_speed;
		direction = new_dir;
	}

	if (keystate[SDL_SCANCODE_E])
	{
		owner.get_component<AnimationController>()->play_animation(5);
	}

	if (mouse & SDL_BUTTON(SDL_BUTTON_LEFT))
	{
		bones_angle = camera->transform.forward.y;
		current_action = Action::Shooting;
		direction = camera->transform.forward;

		if (!firing_bullet)
		{
			firing_bullet = true;
		}
	}
	else if (mouse & SDL_BUTTON(SDL_BUTTON_RIGHT))
	{
		direction = camera->transform.forward;
		bones_angle = camera->transform.forward.y;
		current_action = Action::Aiming;
	}

	if (!(mouse & SDL_BUTTON(SDL_BUTTON_LEFT)))
	{
		firing_bullet = false;
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
		rotate_bones(bones_angle);
		if (forward_speed == 0.0f && body->linear_velocity().y >= -0.01f && body->linear_velocity().y <= 0.01f)
		{
			owner.get_component<AnimationController>()->play_animation(current_animation(), true);
		}
		else
		{
			owner.get_component<AnimationController>()->play_animation(1, true);
		}
		
		if (keystate[SDL_SCANCODE_SPACE])
		{
			
			jump();
		}
		glm::vec3 forward_velocity = direction * forward_speed;
		body->set_velocity(glm::vec3(forward_velocity.x, body->linear_velocity().y, forward_velocity.z));
		direction = glm::normalize(direction);
		direction.y = 0;
		transform.look_at(transform.position + direction);
	}
	if (SDL_GetTicks() - shoot_start_time >= 200 && firing_bullet)
	{
		shoot_start_time = SDL_GetTicks();
		fire_bullet();

	}
	

	current_action = Action::None;
}

void PlayerController::set_camera(Camera* cam)
{
	if (cam == nullptr)
	{
		std::clog << "PlayerController::set_camera: cam is null" << std::endl;
		return;
	}
	camera = cam;
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
		owner.get_component<AnimationController>()->play_animation(3, true);
		std::cout << "Aiming" << std::endl;
	}
	else if (current_action == Action::Shooting)
	{
		index = 4;
		owner.get_component<AnimationController>()->play_animation(4, true);
		std::cout << "Shooting" << std::endl;
	}

	return index;
}

void PlayerController::jump()
{
	//owner.get_component<PhysicsBody>()->apply_impulse(Transform::world_up_vector() * jump_force);
	current_action = Action::Jumping;
	owner.get_component<AnimationController>()->play_animation(2);
}

void PlayerController::fire_bullet()
{
	PhysicsWorld::RayHit* hit = PhysicsWorld::ray_cast(
		camera->transform.position, camera->transform.forward, 3000.0f);

	glm::vec3 bullet_velocity = glm::vec3();
	glm::vec3 bullet_end = glm::vec3();
	if (hit != nullptr)
	{
		bullet_end = hit->hit_point;
		if (hit->entity->get_component<EnemyController>())
		{
			EntityManager::remove_entity(hit->entity);
		}
		else if (hit->entity->get_component<PhysicsBody>())
		{
			hit->entity->get_component<PhysicsBody>()->apply_impulse(
				glm::normalize(bullet_end - camera->transform.position) * 5.0f);
		}
	}
	else
	{
		bullet_end = camera->transform.position + camera->transform.forward * 3000.0f;
	}
	bullet_velocity = glm::normalize(bullet_end - transform.position) * 5.0f;
	Entity& bullet = Spawn::spawn_bullet(transform.position, bullet_velocity, bullet_end, "crosshair.png");
	bullet.transform.look_at(transform.position + transform.right);
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

	return direction;
}