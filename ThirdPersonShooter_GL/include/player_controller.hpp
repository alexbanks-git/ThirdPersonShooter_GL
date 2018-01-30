#pragma once
#include "controller_component.hpp"
#include "camera.hpp"

class PlayerController : public ControllerComponent
{
private:
	Camera* camera;
	enum class Action {None, Shooting, Aiming, Rolling, Jumping};
	Action current_action;
	GLfloat shoot_start_time;
	bool firing_bullet;
	GLfloat jump_force;
	GLfloat run_speed;
	GLfloat walk_speed;

	/**
	 * @brief Rotates the players shoulder and neck bones.
	 * @param angle the amount to rotate the bones
	 */
	void rotate_bones(GLfloat angle);

	/**
	 * @brief Changes the direction that the player is facing based on the key pressed.
	 * @param keys the key state
	 * @returns a direction vector
	 */
	glm::vec3 change_facing_direction(const Uint8* keys);

	/**
	 * @brief Returns the player's current animation based on their Action state
	 * @returns the animation's index
	 */
	GLuint current_animation();

	/**
	 * @brief Performs raycast in players forward direction, and spawns a bullet entity.
	 */
	void fire_bullet();

	/**
	 * @brief Makes the player perform a jump
	 */
	void jump();
public:
	PlayerController(Entity* entity);
	/**
	 * @brief Handles player input 
	 */
	void update();

	/**
	 * @brief Retrieves the type of the class.
	 * @returns the class type
	 */
	std::string type_name();

	/**
	 * @brief Sets the camera that follows the player
	 * @param cam pointer to the camera
	 */
	void set_camera(Camera* cam);
};