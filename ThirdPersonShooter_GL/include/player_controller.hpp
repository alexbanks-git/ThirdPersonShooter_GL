#pragma once
#include "controller_component.hpp"
#include "camera.hpp"

class PlayerController : public ControllerComponent
{
private:
	Camera* camera;
	enum class Action {None, Shooting, Aiming, Rolling, Jumping, Cover};
	enum class PlayerAnimation 
	{
		Idle, 
		Run, 
		Jump, 
		Aim, 
		Shoot, 
		Roll, 
		Crouch_Idle, 
		Crouch_Walk, 
		Cover_Idle, 
		Cover_left,
		Cover_Right,
		Cover_Idle_Opposite
	};

	Action current_action;
	GLfloat shoot_start_time;
	bool firing_bullet;
	GLfloat jump_force;
	GLfloat run_speed;
	GLfloat walk_speed;
	GLfloat cover_timer;
	glm::vec3 cover_move_dir;
	Entity* cover;
	GLfloat cover_speed;
	SDL_GameController* game_controller;
	bool a_button;
	bool b_button;
	Sint16 left_trigger;
	Sint16 right_trigger;
	Sint16 left_stick_x;
	Sint16 left_stick_y;
	glm::vec3 cover_left;
	glm::vec3 cover_right;

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

	void play_animation(PlayerAnimation anim, bool loop = false);
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

	void init_game_controller();
};