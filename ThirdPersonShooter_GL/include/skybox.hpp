#pragma once
#include <vector>
#include <string>
#include "GL/glew.h"
#include "SDL.h"

class Skybox
{
private:
	GLuint texture, vao, vbo;
	SDL_Surface* cube_faces[6];
public:
	/**
	 * @brief Creates the skybox using the given images.
	 * @param images the the images to use for the skybox's faces
	 */
	void generate_skybox(std::vector<std::string> images);

	/**
	 * @brief Draws the skybox to the screen.
	 */
	void draw_skybox();
};