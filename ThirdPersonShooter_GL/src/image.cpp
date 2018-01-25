#include "image.hpp"
#include "SDL_image.h"
#include "graphics.hpp"

Image::Image(Entity* entity) : DrawableComponent(entity)
{
	material = 1;
}

void Image::set_size(GLfloat width, GLfloat height)
{
	GLfloat vertices[] =
	{
		width, height, -1, 1, 1,
		width, -height, -1, 1, 0,
		-width, -height, -1, 0, 0,
		-width, -height, -1, 0, 0,
		-width, height, -1, 0, 1,
		width, height, -1, 1, 1
	};


	glGenBuffers(1, &vbo);
	glGenVertexArrays(1, &vao);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, 0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (const void*)(sizeof(GLfloat) * 3));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Image::set_image_path(std::string path)
{
	image_path = path;
	SDL_Surface* surface = IMG_Load(image_path.c_str());
	SDL_PixelFormat* pixel_format = surface->format;
	GLint type;

	if (pixel_format->Amask)
	{
		type = GL_RGBA;
	}
	else
	{
		type = GL_RGB;
	}
	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, type, surface->w, surface->h, 0, type, GL_UNSIGNED_BYTE, surface->pixels);
	//glGenerateMipmap(GL_TEXTURE_2D);
	SDL_FreeSurface(surface);
}

void Image::draw()
{
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(Graphics::get_user_interface_shader(), "sampler"), 4);
	glBindVertexArray(vao);
	GLuint model_location = glGetUniformLocation(Graphics::get_user_interface_shader(), "model");
	glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(transform.get_transformation()));
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}