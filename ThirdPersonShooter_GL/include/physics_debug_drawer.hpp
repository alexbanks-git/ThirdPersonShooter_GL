#pragma once
#include <vector>
#include <iostream>
#include "GL/glew.h"
#include "btBulletDynamicsCommon.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

class PhysicsDebugDrawer : public btIDebugDraw
{
	GLint debug_mode;
	GLuint vao;
	GLuint vbo;
	std::vector<glm::vec3> vertex_data;

public:
	PhysicsDebugDrawer();

	/**
	 * @brief Draws a line on the screen.
	 * @param from the starting point of the line
	 * @param to the ending point of the line
	 * @param color the color of the line
	 */
	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);

	/**
	 * @brief Sets the drawer's debug mode.
	 * @param debug_mode 
	 */
	void setDebugMode(int debug_mode);

	void draw3dText(const btVector3& location, const char* textString);

	void reportErrorWarning(const char* warningString);

	void drawContactPoint(const btVector3& pointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color);

	virtual void clearLines();

	int getDebugMode() const { return debug_mode; }
};