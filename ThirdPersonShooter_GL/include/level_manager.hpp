#pragma once

class LevelManager
{
public:
	/**
	 * @brief Initializes the level manager.
	 */
	static void init_level();

	/**
	 * @brief Updates all entities in the level.
	 */
	static void update_level();

	/**
	 * @brief Draws all visible entities in the level.
	 */
	static void draw_level();

	/**
	 * @brief Draws the level's skybox.
	 */
	static void draw_skybox();

	/**
	 * @brief Draws all user interface elements.
	 */
	static void draw_ui();

	/**
	 * @brief Draws all non user interface images.
	 */
	static void draw_images();

	/**
	 * @brief Draws all lights in the level.
	 */
	static void draw_lights();

	static void create_shadows();

	static void pause_level(bool pause);
	static bool is_paused();
};