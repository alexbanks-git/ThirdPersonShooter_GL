#include "bullet_controller.hpp"
#include "physics_body.hpp"
#include "entity_manager.hpp"

BulletController::BulletController(Entity* entity) : ControllerComponent(entity)
{

}

void BulletController::update()
{
	/*if (SDL_GetTicks() >= time_to_die)
	{
		EntityManager::remove_entity(&owner);
	}
	else
	{*/
		transform.global_translate(velocity);
	//}
}

void BulletController::set_velocity(glm::vec3 vel)
{
	velocity = vel;
}

void BulletController::set_endpoint(glm::vec3 end)
{
	time_to_die = SDL_GetTicks() + glm::length(end / velocity);
}

std::string BulletController::type_name()
{
	return "BulletController";
}