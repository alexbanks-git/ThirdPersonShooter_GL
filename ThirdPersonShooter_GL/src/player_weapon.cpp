#include "entity.hpp"
#include "player_weapon.hpp"

PlayerWeapon::PlayerWeapon(Entity* entity) : ControllerComponent(entity)
{

}

void PlayerWeapon::update()
{
	recalculate_offsets();
	transform.set_position((offset_position + parent->transform.position + local_position));
	transform.set_rotation(offset_rotation);
}

void PlayerWeapon::attach_to_entity(Entity* entity, glm::mat4* trans)
{
	parent = entity;
	offset = trans;
	local_position = transform.position;
	recalculate_offsets();
}

void PlayerWeapon::recalculate_offsets()
{
	offset_rotation = (glm::mat3)(*offset);
	offset_position.x = (*offset)[3][0];
	offset_position.y = (*offset)[3][1];
	offset_position.z = (*offset)[3][2];
}

std::string PlayerWeapon::type_name()
{
	return "PlayerWeapon";
}