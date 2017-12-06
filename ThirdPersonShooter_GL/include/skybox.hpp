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
	void generate_skybox(std::vector<std::string> images);
	void draw_skybox();
};