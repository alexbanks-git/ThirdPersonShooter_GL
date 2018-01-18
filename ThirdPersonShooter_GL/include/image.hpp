#pragma once
#include "drawable_component.hpp"
class Image : public DrawableComponent
{
private:
	GLuint vbo;
	GLuint vao;
	GLuint texture;
	std::string image_path;
public:
	Image(Entity* entity);

	/**
	 * @brief Draws the image to the screen.
	 */
	void draw();

	/**
	 * @brief Sets the file path of the texture to use.
	 * @param path the file path of the texture
	 */
	void set_image_path(std::string path);

	/**
	 * @brief Sets the size of the image, and creates vertex buffer.
	 * @param width the width of the image
	 * @param height the height of the image
	 */
	void set_size(GLfloat width, GLfloat height);
};