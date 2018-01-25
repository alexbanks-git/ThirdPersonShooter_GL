#pragma once
#include "camera.hpp"
#include "entity_manager.hpp"

class Spawn
{
public:
	/**
	 * @brief Spawns the player in the world.
	 * @param position the location to spawn the player at
	 * @param camera the camera that follows the player
	 * @returns a reference to the player
	 */
	static Entity& spawn_player(glm::vec3 position, Camera* camera);

	/**
	 * @brief Spawns a basic enemy in the world.
	 * @param position the location to spawn the enemy at
	 * @param target the target that the enemy should attack
	 * @returns a reference to the enemy
	 */
	static Entity& spawn_runner(glm::vec3 position, Entity* target);

	/**
	 * @brief Spawns a bullet in the world.
	 * @param position the location to spawn the bullet at
	 * @param velocity the velocity of the bullet
	 * @param end the position that the bullet dies at
	 * @param path the file path of the bullet's image
	 * @returns a reference to the bullet
	 */
	static Entity& spawn_bullet(glm::vec3 position, glm::vec3 velocity, glm::vec3 end, std::string path);

	/**
	 * @brief Spawns an infinite plane in the world.
	 * @param y_position the position on the y-axis to draw the plane at
	 * @returns a reference to the plane
	 */
	static Entity& spawn_plane(GLfloat y_position);

	/**
	 * @brief Spawns a cube in the world.
	 * @param position the location to spawn the cube at
	 * @param is_static whether or not the cube should be a static object
	 * @returns a reference to the cube
	 */
	static Entity& spawn_cube(glm::vec3 position, bool is_static);

	/**
	 * @brief Spawns a sphere in the world.
	 * @param position the location to spawn the sphere at
	 * @param is_static whether or not the sphere should be a static object
	 * @returns a reference to the sphere
	 */
	static Entity& spawn_sphere(glm::vec3 position, bool is_static);

	/**
	 * @brief Spawns a 2D image in the world.
	 * @param position the location to spawn the image at
	 * @param path the file path of the texture to use 
	 * @returns a reference to the image
	 */
	static Entity& spawn_image(glm::vec3 position, std::string path);

	/**
	 * @brief Spawns a camera in the world.
	 * @param position the location to spawn the camera at
	 * @param field_of_view the field of view of the camera
	 * @param near the near clipping plane of the camera
	 * @param far the far clipping plane of the camera
	 * @returns a reference to the camera
	 */
	static Entity& spawn_camera(glm::vec3 position, GLfloat field_of_view, GLfloat near, GLfloat far);

	static Entity& spawn_text(glm::vec3 position, std::string path);

	static Entity& spawn_model(glm::vec3 position, std::string model_path);

	static Entity& spawn_weapon(glm::vec3 position, std::string model_path, Entity* parent, glm::mat4* offset);

	static Entity& spawn_walker(glm::vec3 position, Entity* target);

	static Entity& spawn_crawler(glm::vec3 position, Entity* target);
};